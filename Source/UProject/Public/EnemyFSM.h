// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Search,
	Move,
	Attack,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	void IdleState();
	void SearchState();
	void MoveState();
	void AttackState();
	void ChangeState(EEnemyState afterState);
	bool FlowTime(float delayTime);
	void ReturnHand();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState state;

	UPROPERTY(EditAnywhere)
		class AVRPlayer* player;

	UPROPERTY(EditAnywhere)
		class AEnemy* enemy;

	UPROPERTY(EditAnywhere)
		class ABall* ball;

	UPROPERTY(EditAnywhere)
		class AMovePoint* point;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class ABlueGoalPost* blueGoalPost;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* moveEffect;


	FTimerHandle returnHandle;

public:

	UPROPERTY(EditAnywhere, Category = FSM)
		float idleDelayTIme = 2;


	UPROPERTY(EditAnywhere, Category = FSM)
		float searchDelayTIme = 2;

	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTIme = 3;

	float currentTime = 0;

	UPROPERTY(EditAnywhere, Category = FSM)
		float searchRange = 1000;

	UPROPERTY(EditAnywhere, Category = FSM)
		float moveSpeed = 0;
	
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackSpeed = 0;

		FVector returnHandLoc;
	int32 attack = 0;
	float x;
	float y;
	float z;

};
