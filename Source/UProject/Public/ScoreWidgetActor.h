// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreWidgetActor.generated.h"

UCLASS()
class UPROJECT_API AScoreWidgetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoreWidgetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* scoreWG;

	UPROPERTY(EditAnywhere)
	class UScoreWidget* scoreUI;

	void ReceiveBlueScore(int32 score);
	void ReceiveRedScore(int32 score);
};
