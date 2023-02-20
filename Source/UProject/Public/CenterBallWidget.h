// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CenterBallWidget.generated.h"

/**
 * 
 */
UCLASS()
class UPROJECT_API UCenterBallWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* CountDown;

	

};
