// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemFactory.generated.h"

UCLASS()
class UPROJECT_API AItemFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class AItem* item;

	UPROPERTY(EditAnywhere, Category = SpawnSetting)
	TSubclassOf<class AItem> ItemFactory;

	void SpawnBall();

	FTimerHandle SpawnTimerHandle;

	float currTime = 0;
	float createTime = 0;
};
