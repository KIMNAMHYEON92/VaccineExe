#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

UCLASS()
class VACCINEEXE_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* scoreData;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* hpData;

	void UpdateScore(int32 NewScore);
	void UpdateHP(int32 CurrentHP);
};