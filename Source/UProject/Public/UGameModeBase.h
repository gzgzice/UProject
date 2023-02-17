// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UPROJECT_API AUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUGameModeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	class UScoreWidget* scoreUI;	
	
	UPROPERTY(EditAnywhere)
	class AScoreWidgetActor* ScoreWidgetActor;

	int32 currRedScore = 0;
	int32 currBlueScore = 0;

	void AddRedScore(int32 newscore);
	void AddBlueScore(int32 newscore);
};
