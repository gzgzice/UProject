// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoalPost.h"
#include "RedGoalPost.generated.h"

/**
 * 
 */
UCLASS()
class UPROJECT_API ARedGoalPost : public AGoalPost
{
	GENERATED_BODY()
	
public:

	ARedGoalPost();

	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void BallOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	
	UPROPERTY(EditAnywhere)
	class UParticleSystem* goalEffect;
};
