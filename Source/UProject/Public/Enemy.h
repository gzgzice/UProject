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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSMComponent)
		class UEnemyFSM* fsm;

	UPROPERTY(EditAnywhere)
		class UChildActorComponent* rightChild;

	UPROPERTY(EditAnywhere)
		class UChildActorComponent* leftChild;

	UPROPERTY(EditAnywhere, Category = FSMComponent)
		TSubclassOf<class ARightHand> rightHand;

	UPROPERTY(EditAnywhere, Category = FSMComponent)
		TSubclassOf<class ALeftHand> leftHand;



};
