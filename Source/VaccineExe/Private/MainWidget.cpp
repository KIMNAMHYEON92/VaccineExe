#include "MainWidget.h"
#include "Components/TextBlock.h"

void UMainWidget::UpdateScore(int32 NewScore)
{
	if (scoreData)
	{
		scoreData->SetText(FText::AsNumber(NewScore));
	}
}

void UMainWidget::UpdateHP(int32 CurrentHP)
{
	if (hpData)
	{
		hpData->SetText(FText::AsNumber(CurrentHP));
	}
}