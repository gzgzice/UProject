// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "testPlayer.generated.h"

UCLASS()
class UPROJECT_API AtestPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AtestPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UCameraComponent* Cam;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UMotionControllerComponent* rightMotionController;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UMotionControllerComponent* leftMotionController;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UStaticMeshComponent* headMesh;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class USkeletalMeshComponent* leftHand;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class USkeletalMeshComponent* rightHand;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UTextRenderComponent* leftLog;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UTextRenderComponent* rightLog;

	UPROPERTY(EditAnywhere, Category = "VR_Setting | Component")
		TEnumAsByte <enum EHMDTrackingOrigin::Type> trackOrigin;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		class UInputMappingContext* myMapping;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		class UInputAction* leftActionX;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		float fireDistance = 50.0f;

private:
	void OnLeftActionX();
	void ReleaseActionX();
	void DrawLocationLine();

	bool bIsDraw = false;

};
