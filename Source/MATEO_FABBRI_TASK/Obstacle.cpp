// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"

#include "MATEO_FABBRI_TASKCharacter.h"

// Sets default values
AObstacle::AObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionTrigger"));
	SetRootComponent(BoxCollision);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxCollision);
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &AObstacle::OnOverlapBegin);
}

void AObstacle::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
                               AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                               bool bFromSweep,
                               const FHitResult& SweepResult)
{
	AMATEO_FABBRI_TASKCharacter* Character = Cast<AMATEO_FABBRI_TASKCharacter>(OtherActor);

	if (!Character)
		return;

	Character->AddScore(Score);

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AObstacle::EnableCollision, ScoreDelay, false);
}

void AObstacle::EnableCollision() const
{
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
