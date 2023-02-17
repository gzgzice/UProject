// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DirectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class UPROJECT_API UDirectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//UDirectionWidget();

	//virtual void NativeConstruct() override;
	
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Arrow_Img;
 
	void ArrowRotation(float angle);
};
