// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class UPROJECT_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(Editanywhere)
	class USphereComponent* ball;

// 	UPROPERTY(Editanywhere)
// 		class AEnemy* enemy;

	UPROPERTY(Editanywhere)
		class AVRPlayer* player;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACenterBallWidgetActor> widgetActor;

	UPROPERTY(EditAnywhere)
		FVector	startPos;

	UPROPERTY(EditAnywhere)
		FVector	endPos;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		class UHapticFeedbackEffect_Base* goalHaptic;

	void CenterBall();
	void StartBall();

	FTimerHandle goalHandle;
	FTimerHandle startHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bGoal = false;

	UPROPERTY(EditAnywhere)
	TArray<class AEnemy*> enemyArray;

};
