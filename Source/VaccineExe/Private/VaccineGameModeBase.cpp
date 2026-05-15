// Fill out your copyright notice in the Description page of Project Settings.


#include "VaccineExe/Public/VaccineGameModeBase.h"

void AVaccineGameModeBase::AddScore(int32 point)
{
	currentScore += point;
	UE_LOG(LogTemp, Warning, TEXT("Current Score: %d"), currentScore);
}
