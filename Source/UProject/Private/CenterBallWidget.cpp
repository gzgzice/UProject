// Fill out your copyright notice in the Description page of Project Settings.


#include "CenterBallWidget.h"
#include <UMG/Public/Components/TextBlock.h>

void UCenterBallWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//CountDown = Cast<UTextBlock>(GetWidgetFromName(TEXT("CountDown")));
}

void UCenterBallWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	currTime += InDeltaTime;
	if (currTime == 1)
	{
		CountDown->SetText(FText::FromString("2"));
	}
	else if (currTime == 2)
	{
		CountDown->SetText(FText::FromString("1"));
	}
	else if (currTime == 3)
	{
		CountDown->SetText(FText::FromString("Go!!"));
	}
}
