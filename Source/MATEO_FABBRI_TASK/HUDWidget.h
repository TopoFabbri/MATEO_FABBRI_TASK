// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MATEO_FABBRI_TASK_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

	virtual void NativeConstruct() override;

public:
	void SetScore(int Score) const;
};
