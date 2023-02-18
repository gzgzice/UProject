// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class UPROJECT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeBeginPlay() override;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEnemyState state;

	UPROPERTY(EditAnywhere)
	class AEnemy* enemy;
};
