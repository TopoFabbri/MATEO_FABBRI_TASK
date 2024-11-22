// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SkaterHUD.generated.h"

/**
 * 
 */
UCLASS()
class MATEO_FABBRI_TASK_API ASkaterHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> HudWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UHUDWidget* HudWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UPauseWidget* PauseWidget;
	
	virtual void BeginPlay() override;

public:
	void UpdateScore(int Score) const;
	void SetPause(bool bPause) const;
};
