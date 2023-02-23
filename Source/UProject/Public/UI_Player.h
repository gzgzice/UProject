// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UI_Player.generated.h"

UCLASS()
class UPROJECT_API AUI_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUI_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UMotionControllerComponent* leftMotionController;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UMotionControllerComponent* rightMotionController;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UCameraComponent* Cam;

	UPROPERTY(EditAnyWhere, Category = "VR_Settings | Components")
		class UStaticMeshComponent* headMesh;

	UPROPERTY(EditAnyWhere, Category = "VR_Settings | Components")
		class USkeletalMeshComponent* leftHand;

	UPROPERTY(EditAnyWhere, Category = "VR_Settings | Components")
		class USkeletalMeshComponent* rightHand;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		class UInputMappingContext* myMapping;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Component")
		class UWidgetInteractionComponent* widgetPointer_right;	

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Component")
		class UWidgetPointerComponent* widgetPointerComp;
};
