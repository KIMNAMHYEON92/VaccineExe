// Fill out your copyright notice in the Description page of Project Settings.


#include "VaccineExe/Public/VaccineGameModeBase.h"
#include "MenuWidget.h"
#include "MainWidget.h"
#include "PlayerPawn.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AVaccineGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (mainWidgetClass)
	{
		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidgetClass);
		if (mainUI)
		{
			mainUI->AddToViewport();
			mainUI->UpdateScore(currentScore);
			
			// 초기 HP 표시
			APlayerPawn* Player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			if (Player)
			{
				mainUI->UpdateHP(Player->hp);
			}
		}
	}
}

void AVaccineGameModeBase::AddScore(int32 point)
{
	currentScore += point;
	UE_LOG(LogTemp, Warning, TEXT("Current Score: %d"), currentScore);

	if (mainUI)
	{
		mainUI->UpdateScore(currentScore);
	}

	// 성장 로직
	APlayerPawn* Player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player)
	{
		if (currentScore >= 50)
		{
			Player->UpgradeWeapon(3);
		}
		else if (currentScore >= 20)
		{
			Player->UpgradeWeapon(2);
		}
	}
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
