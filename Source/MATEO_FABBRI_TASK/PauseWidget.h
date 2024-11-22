// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class MATEO_FABBRI_TASK_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void QuitGame();
};
