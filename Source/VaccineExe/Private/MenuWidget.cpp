#include "MenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (button_Restart)
	{
		button_Restart->OnClicked.AddDynamic(this, &UMenuWidget::Restart);
	}

	if (button_Quit)
	{
		button_Quit->OnClicked.AddDynamic(this, &UMenuWidget::Quit);
	}
}

void UMenuWidget::Restart()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMap"));
}

void UMenuWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}