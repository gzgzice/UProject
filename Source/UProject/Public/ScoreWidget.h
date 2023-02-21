// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

UCLASS()
class UPROJECT_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* RedScore;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* BlueScore;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Timer;

	void UpdateRedScoreUI(int32 score);
	void UpdateBlueScoreUI(int32 score);
	void GetText();
	void StartTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 currRedScore = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 currBlueScore = 0;
	int32 min = 3;
	int32 sec = 0;

};
