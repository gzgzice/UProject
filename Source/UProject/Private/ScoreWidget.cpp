// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"
#include "Components/TextBlock.h"

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UScoreWidget::UpdateRedScoreUI(int32 score)
{
	currRedScore += score;

	RedScore->SetText(FText::AsNumber(currRedScore));
}

void UScoreWidget::UpdateBlueScoreUI(int32 score)
{
	currBlueScore += score;

	BlueScore->SetText(FText::AsNumber(currBlueScore));
}