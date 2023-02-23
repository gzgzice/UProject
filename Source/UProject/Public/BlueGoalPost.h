// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlueGoalPost.generated.h"

UCLASS()
class UPROJECT_API ABlueGoalPost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlueGoalPost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere)
		class UBoxComponent* goalPost;

	UPROPERTY(EditAnywhere)
		class ABall* ball;

	UPROPERTY(EditAnywhere)
		class UScoreWidget* scoreWidget;

	UPROPERTY(EditAnywhere)
		class AScoreWidgetActor* ScoreWidgetActor;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ARedGoalParticleActor> goalEffect;

	UPROPERTY(EditAnywhere)
		class USoundBase* goalSound;

	UPROPERTY(EditAnywhere)
		TArray <class AScoreWidgetActor*> WidgetActorArray;

public:

	UFUNCTION()
		void BallOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};




