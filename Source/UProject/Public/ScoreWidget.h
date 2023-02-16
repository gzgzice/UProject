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
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* RedScore;	
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* BlueScore;

	void UpdateRedScoreUI(int32 score);
	void UpdateBlueScoreUI(int32 score);

	int32 currRedScore = 0;
	int32 currBlueScore = 0;

};
