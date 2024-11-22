// Fill out your copyright notice in the Description page of Project Settings.


#include "SkaterHUD.h"

#include "HUDWidget.h"
#include "PauseWidget.h"
#include "Blueprint/UserWidget.h"

void ASkaterHUD::BeginPlay()
{
	Super::BeginPlay();

	check(HudWidgetClass);
	check(PauseWidgetClass);

	HudWidget = CreateWidget<UHUDWidget>(GetWorld(), HudWidgetClass);
	PauseWidget = CreateWidget<UPauseWidget>(GetWorld(), PauseWidgetClass);

	if (HudWidget)
		HudWidget->AddToViewport();
}

void ASkaterHUD::UpdateScore(const int Score) const
{
	if (HudWidget)
		HudWidget->SetScore(Score);
}

void ASkaterHUD::SetPause(bool bPause) const
{
	if (!PauseWidget)
		return;

	if (bPause)
		PauseWidget->AddToViewport();
	else
		PauseWidget->RemoveFromParent();
}
