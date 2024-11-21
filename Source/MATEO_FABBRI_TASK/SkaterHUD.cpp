// Fill out your copyright notice in the Description page of Project Settings.


#include "SkaterHUD.h"

#include "HUDWidget.h"
#include "Blueprint/UserWidget.h"

void ASkaterHUD::BeginPlay()
{
	Super::BeginPlay();

	check(HudWidgetClass);

	HudWidget = CreateWidget<UHUDWidget>(GetWorld(), HudWidgetClass);

	if (HudWidget)
		HudWidget->AddToViewport();
}

void ASkaterHUD::UpdateScore(const int Score) const
{
	if (HudWidget)
		HudWidget->SetScore(Score);
}
