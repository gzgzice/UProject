// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class UPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSMComponent)
		class UEnemyFSM* fsm;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 		class UChildActorComponent* hand;
// 
// 	UPROPERTY(EditAnywhere, Category = FSMComponent)
// 		TSubclassOf<class AHand> child;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* hand;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* handMesh;

	UPROPERTY(EditAnywhere)
		class ABall* ball;

public:

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	
	UPROPERTY(EditAnywhere)
		float ballMass = 1500;

		bool bHit = false;
};
