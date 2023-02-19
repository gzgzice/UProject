// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovePoint.generated.h"

UCLASS()
class UPROJECT_API AMovePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovePoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UCapsuleComponent* caughtBox;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UStaticMeshComponent* compMesh;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UStaticMeshComponent* compMesh0;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UMaterial* floorMat;

};
