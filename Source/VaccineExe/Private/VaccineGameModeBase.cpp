// Fill out your copyright notice in the Description page of Project Settings.


#include "VaccineExe/Public/VaccineGameModeBase.h"
#include "MenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AVaccineGameModeBase::AddScore(int32 point)
{
	currentScore += point;
	UE_LOG(LogTemp, Warning, TEXT("Current Score: %d"), currentScore);
}

void AVaccineGameModeBase::ShowMenu()
{
	if (menuWidgetClass)
	{
		UMenuWidget* menu = CreateWidget<UMenuWidget>(GetWorld(), menuWidgetClass);
		if (menu)
		{
			menu->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}
	}
}
