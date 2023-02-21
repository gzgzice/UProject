// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GoalKeeper.generated.h"

UCLASS()
class UPROJECT_API AGoalKeeper : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGoalKeeper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* hand;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* handMesh;

	UPROPERTY(EditAnywhere)
		class ABall* ball;

	UPROPERTY(EditAnywhere)
		class USoundBase* hitBallSound;

public:

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ReturnHand();
	void BlockHand(float speed);
public:

	float ballMass = 1500;
	float blockSpeed;
	bool bHitBall = false;
	bool bHitOther = false;
	FVector dir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBlock = false;
	float clamp;
};
