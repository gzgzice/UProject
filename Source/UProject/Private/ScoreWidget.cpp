// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UScoreWidget::UpdateRedScoreUI(int32 score)
{
	RedScore->SetText(FText::AsNumber(score));
}

void UScoreWidget::UpdateBlueScoreUI(int32 score)
{
	BlueScore->SetText(FText::AsNumber(score));
}