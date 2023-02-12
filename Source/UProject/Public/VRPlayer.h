// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "VRPlayer.generated.h"

UCLASS()
class UPROJECT_API AVRPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRPlayer();

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
		class UInputAction* RightThumbStick;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		class UInputAction* rightTrigger;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		float fireDistance = 50.0f;

private:
	void OnLeftActionX();
	void ReleaseActionX();
	void RotateAxis(const struct FInputActionValue& value);
	void DrawLocationLine();
	void FireRightHand(const struct FInputActionValue& value);
	void FireHand(float deltatime);
	void ReturnRightHand();
	void ReturnMove(float deltatime);

	bool bIsDraw = false;
	bool bIsFire = false;
	bool bIsReturn = false;
	FVector currentPos;
	FVector startPos;
	float goalDir = 500.0f;

	UPROPERTY(EditAnywhere)
	float speed = 100;
	float axis = 0;
	float currtime;
	FVector handPos;

	TArray<FVector> lineLoc;
};
