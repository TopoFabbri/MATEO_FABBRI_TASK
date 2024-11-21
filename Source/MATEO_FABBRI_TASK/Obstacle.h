// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class MATEO_FABBRI_TASK_API AObstacle : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle")
	int Score = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle")
	float ScoreDelay = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle")
	FTimerHandle TimerHandle;
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void EnableCollision() const;

public:
};
