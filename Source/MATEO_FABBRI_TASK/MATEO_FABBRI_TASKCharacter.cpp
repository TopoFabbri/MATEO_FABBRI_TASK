// Copyright Epic Games, Inc. All Rights Reserved.

#include "MATEO_FABBRI_TASKCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AMATEO_FABBRI_TASKCharacter::AMATEO_FABBRI_TASKCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	SkateStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateStaticMesh"));
	SkateStaticMesh->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMATEO_FABBRI_TASKCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	SkaterHUD = Cast<ASkaterHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
}

void AMATEO_FABBRI_TASKCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetMinimumVelocity();
	CalculateForwardVelocity();
	PositionSkateMesh();

	if (bOnAir && !GetCharacterMovement()->IsFalling())
		OnLand();

	if (GetCharacterMovement()->GetCurrentAcceleration().Size2D() <= 0.f)
		bShouldKick = false;
}

void AMATEO_FABBRI_TASKCharacter::CalculateForwardVelocity() const
{
	if (GetCharacterMovement()->IsFalling())
		return;

	FVector SkateForward = SkateStaticMesh->GetForwardVector();

	FVector updatedVelocity = GetCharacterMovement()->Velocity;
	updatedVelocity.Z = 0;
	updatedVelocity = SkateForward * updatedVelocity.Size();

	updatedVelocity.Z = GetCharacterMovement()->Velocity.Z;
	GetCharacterMovement()->Velocity = updatedVelocity;
}

void AMATEO_FABBRI_TASKCharacter::SetMinimumVelocity()
{
	if (GetCharacterMovement()->IsFalling())
		return;

	if (GetCharacterMovement()->Velocity.Size() < MinForwardVelocity)
	{
		GetCharacterMovement()->Velocity = SkateStaticMesh->GetForwardVector() * MinForwardVelocity;
		bShouldKick = false;
	}
}

void AMATEO_FABBRI_TASKCharacter::PositionSkateMesh() const
{
	FVector FW = SkateStaticMesh->GetSocketLocation("FW");
	FVector BW = SkateStaticMesh->GetSocketLocation("BW");

	FHitResult HitResult;
	FVector StartLocation = FW + FVector(0, 0, 30.f);
	FVector EndLocation = FW + FVector(0, 0, -30.f);
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, SkateStaticMesh->GetOwner());
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bDebugQuery = true;

	FVector FWHitLocation;
	FVector BWHitLocation;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams))
		FWHitLocation = HitResult.Location;
	else
		return;

	StartLocation = BW + FVector(0, 0, 30.f);
	EndLocation = BW + FVector(0, 0, -30.f);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams))
		BWHitLocation = HitResult.Location;
	else
		return;

	FRotator Rotation = SkateStaticMesh->GetComponentRotation();
	Rotation = FMath::Lerp(Rotation, (FWHitLocation - BWHitLocation).Rotation(),
	                       SkateRotSpeed * GetWorld()->GetDeltaSeconds());

	SkateStaticMesh->SetWorldRotation(Rotation);
}

void AMATEO_FABBRI_TASKCharacter::ResetSpin()
{
	Spin = 0.f;
}

void AMATEO_FABBRI_TASKCharacter::Fall()
{
	PreviousLocation = GetMesh()->GetRelativeLocation();
	PreviousRotation = GetMesh()->GetRelativeRotation();

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMATEO_FABBRI_TASKCharacter::GetUp, FallenTime, false);
}

void AMATEO_FABBRI_TASKCharacter::GetUp()
{
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetMesh()->SetRelativeRotation(PreviousRotation);
	GetMesh()->SetRelativeLocation(PreviousLocation);

	ResetScore();
}

void AMATEO_FABBRI_TASKCharacter::ResetScore()
{
	ScoreCount = 0;

	SkaterHUD->UpdateScore(ScoreCount);
}

bool AMATEO_FABBRI_TASKCharacter::GoodLand(float Rotation) const
{
	while (Rotation > 360.f)
		Rotation -= 360.f;

	while (Rotation < 0.f)
		Rotation += 360.f;

	return Rotation < GoodLandRange / 2.f || Rotation > 360.f - GoodLandRange / 2.f;
}

bool AMATEO_FABBRI_TASKCharacter::MidLand(float Rotation) const
{
	while (Rotation > 360.f)
		Rotation -= 360.f;

	while (Rotation < 0.f)
		Rotation += 360.f;

	return Rotation < MidLandRange / 2.f || Rotation > 360.f - MidLandRange / 2.f;
}

float AMATEO_FABBRI_TASKCharacter::GetNormalizedSpeed() const
{
	return (GetCharacterMovement()->Velocity.Size2D() - MinForwardVelocity) / (GetCharacterMovement()->MaxWalkSpeed -
		MinForwardVelocity);
}

FVector AMATEO_FABBRI_TASKCharacter::GetFrontFootPosition() const
{
	return SkateStaticMesh->GetSocketLocation("FF");
}

void AMATEO_FABBRI_TASKCharacter::AddScore(int Score)
{
	ScoreCount += Score;

	SkaterHUD->UpdateScore(ScoreCount);
}

void AMATEO_FABBRI_TASKCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		                                   &AMATEO_FABBRI_TASKCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
		                                   &AMATEO_FABBRI_TASKCharacter::Look);

		// Pause
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this,
		                                   &AMATEO_FABBRI_TASKCharacter::Pause);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void AMATEO_FABBRI_TASKCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		if (MovementVector.Y <= 0.f)
			bShouldKick = false;

		// get forward vector
		const FVector ForwardDirection = SkateStaticMesh->GetForwardVector();

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (MovementVector.Y > 0.f)
			bShouldKick = true;

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMATEO_FABBRI_TASKCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (GetCharacterMovement()->IsFalling())
		{
			const float DT = GetWorld()->GetDeltaSeconds();

			RootComponent->AddWorldRotation(FRotator(0, LookAxisVector.X * AirRotationSpeed * DT, 0));
			Spin += LookAxisVector.X * AirRotationSpeed * DT;
		}
		else
		{
			const float RotationSpeedAlpha = FMath::Lerp(MaxRotationSpeed, MinRotationSpeed, GetNormalizedSpeed());

			AddControllerYawInput(LookAxisVector.X * RotationSpeedAlpha);
		}

		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMATEO_FABBRI_TASKCharacter::Pause()
{
	SetPause(true);
}

void AMATEO_FABBRI_TASKCharacter::SetPause(const bool bPause) const
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!PlayerController)
		return;
	
	PlayerController->SetPause(bPause);
	SkaterHUD->SetPause(bPause);

	PlayerController->SetShowMouseCursor(bPause);
}

void AMATEO_FABBRI_TASKCharacter::Jump()
{
	Super::Jump();

	bOnAir = true;

	bUseControllerRotationYaw = false;
}

void AMATEO_FABBRI_TASKCharacter::OnLand()
{
	bOnAir = false;
	bUseControllerRotationYaw = true;
	
	if (GoodLand(Spin))
		AddScore(SpinScore);
	else if (MidLand(Spin))
		AddScore(SpinScore / 2);
	else
		Fall();

	ResetSpin();
}