// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RightHand.generated.h"

UCLASS()
class UPROJECT_API ARightHand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARightHand();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere)
		class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* compMesh;

};
