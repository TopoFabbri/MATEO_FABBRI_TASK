// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"

#include "MATEO_FABBRI_TASKCharacter.h"
#include "MATEO_FABBRI_TASKGameMode.h"
#include "Kismet/GameplayStatics.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize buttons
	if ((ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("ResumeButton")))))
		ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::ResumeGame);

	if ((QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("QuitButton")))))
		QuitButton->OnClicked.AddDynamic(this, &UPauseWidget::QuitGame);
}

void UPauseWidget::ResumeGame()
{
	if (AMATEO_FABBRI_TASKCharacter* Character = Cast<AMATEO_FABBRI_TASKCharacter>(
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		Character->SetPause(false);
}

void UPauseWidget::QuitGame()
{
	AMATEO_FABBRI_TASKGameMode* GameMode = Cast<AMATEO_FABBRI_TASKGameMode>(GetWorld()->GetAuthGameMode());

	if (!GameMode)
		return;

	GameMode->ResetLevel();
}
