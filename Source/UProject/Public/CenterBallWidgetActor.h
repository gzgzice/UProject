// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CenterBallWidgetActor.generated.h"

UCLASS()
class UPROJECT_API ACenterBallWidgetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACenterBallWidgetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
		class UWidgetComponent* centerWG;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UWidgetComponent> widgetComp;

	UPROPERTY(EditAnywhere)
		class UCenterBallWidget* centerUI;
};
