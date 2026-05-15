#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

UCLASS()
class VACCINEEXE_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* button_Restart;

	UPROPERTY(meta = (BindWidget))
	class UButton* button_Quit;

	UFUNCTION()
	void Restart();

	UFUNCTION()
	void Quit();
};