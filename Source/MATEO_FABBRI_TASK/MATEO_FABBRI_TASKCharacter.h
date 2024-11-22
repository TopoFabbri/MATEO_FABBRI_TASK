// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SkaterHUD.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MATEO_FABBRI_TASKCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMATEO_FABBRI_TASKCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Skate Static Mesh Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SkateStaticMesh;

	bool bOnAir = false;
	bool bShouldKick = false;
	int ScoreCount = 0;
	float Spin = 0.f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MinForwardVelocity = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MinRotationSpeed = .5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MaxRotationSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float AirRotationSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Score")
	float SpinScore = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Score")
	float MinToScoreSpin = 360.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skate")
	float SkateRotSpeed = 90.f;
		
	UPROPERTY()
	ASkaterHUD* SkaterHUD;
	
public:
	AMATEO_FABBRI_TASKCharacter();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void Jump() override;

	void OnLand();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void CalculateForwardVelocity() const;
	void SetMinimumVelocity();
	void PositionSkateMesh() const;
	void ResetSpin();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	float GetNormalizedSpeed();
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetShouldKick() const { return bShouldKick; }
	FORCEINLINE int GetScoreCount() const { return ScoreCount; }

	void AddScore(int Score);
};
