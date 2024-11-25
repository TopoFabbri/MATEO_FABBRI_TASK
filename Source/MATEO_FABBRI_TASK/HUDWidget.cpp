// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetScore(0);
}

void UHUDWidget::SetScore(const int Score) const
{
	FString ScoreString = "Score: ";
	ScoreString.Append(FString::FromInt(Score));
	
	ScoreText->SetText(FText::FromString(ScoreString));
}
