// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"
#include "Components/TextBlock.h"
#include <Kismet/KismetTextLibrary.h>
#include <Kismet/KismetStringLibrary.h>
#include <Kismet/KismetSystemLibrary.h>

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//UKismetSystemLibrary::K2_SetTimer(this,TEXT("StartTimer"),1,true);

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

void UScoreWidget::GetText()
{
	// 	FString minute = UKismetTextLibrary::Conv_TextToString(UKismetTextLibrary::Conv_IntToText(min,2,2));
	// 	FString second = UKismetTextLibrary::Conv_TextToString(UKismetTextLibrary::Conv_IntToText(sec,2,2));

	FString minute = FString::FromInt(min);
	FString second = FString::FromInt(sec);

	FString timer = minute.Append(TEXT(":")).Append(second);
	Timer->SetText(FText::FromString(timer));
}

void UScoreWidget::StartTimer()
{
// 	if (sec == 0)
// 	{
// 		if (min == 0)
// 		{
// 
// 		}
// 		else
// 		{
// 			sec = 59;
// 			min -= 1;
// 		}
// 	}
// 	else
// 	{
// 		sec -= 1;
// 	}
// 	float RemainingTime = FMath::Max(0.0f, StartTime - UGameplayStatics::GetRealTimeSeconds());
// 	int32 RemainingMinutes = FMath::FloorToInt(RemainingTime / 60.0f);
// 	int32 RemainingSeconds = FMath::FloorToInt(RemainingTime - RemainingMinutes * 60.0f);
// 	FString TimeString = FString::Printf(TEXT("%02d:%02d"), RemainingMinutes, RemainingSeconds);
// 	Timer->SetText(FText::FromString(TimeString));
}
