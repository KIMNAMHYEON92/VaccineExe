// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VaccineGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VACCINEEXE_API AVaccineGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 currentScore = 0;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UMainWidget> mainWidgetClass;

	UPROPERTY()
	class UMainWidget* mainUI;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UMenuWidget> menuWidgetClass;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 point);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowMenu();
};
