// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

UCLASS()
class UPROJECT_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

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
		class UWidgetComponent* widgetComp;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UMotionControllerComponent* rightMotionController;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UMotionControllerComponent* leftMotionController;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UStaticMeshComponent* headMesh;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UStaticMeshComponent* leftHand;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UStaticMeshComponent* rightHand;

 	UPROPERTY(EditAnywhere, Category = "VR_Setting | Component")
 		TEnumAsByte <enum EHMDTrackingOrigin::Type> trackOrigin;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		class UInputMappingContext* myMapping;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		TArray<class UInputAction*> leftInputs;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		TArray<class UInputAction*> rightInputs;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		class UDirectionWidget* dirUI;

	UPROPERTY(EditAnywhere)
		class ABall* ball;

	UPROPERTY(EditAnywhere)
		class ARedGoalPost* RedGoalPost;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* hitEffect;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class APlayerEffectActor> fireEffect;

	UPROPERTY(EditAnywhere)
		class USoundBase* fireSound;

	UPROPERTY(EditAnywhere)
		class USoundBase* moveSound;

	UPROPERTY(EditAnywhere)
		class USoundBase* hitSound;

	UPROPERTY(EditAnywhere)
		class USoundBase* powerhitSound;

	UPROPERTY(EditAnywhere)
		class USoundBase* explosionSound;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* Effect_L;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* Effect_R;

	UPROPERTY(EditAnywhere)
		class UWidgetInteractionComponent* widgetPointer_right;
	void OnleftActionX();
	void ReleaseActionX();
	void OnRightActionA();
	void ReleaseActionA();
	void DrawLocationLine(UMotionControllerComponent* motionController);
public:
	UPROPERTY(EditAnywhere)
		FVector orginPos;

	void ResetPos();

public:
	void RotateRight();
	void RotateLeft();
	void FireLeftHand(const struct FInputActionValue& value);
	void FireRightHand(const struct FInputActionValue& value);
	void LeftHandMove(float deltatime);
	void RightHandMove(float deltatime);
	void ReturnLeftHand();
	void ReturnRightHand();
	void ReturnMove(float deltatime, FVector startPos, FVector currPos, UStaticMeshComponent* handmesh);
	void FindAngle();
	void DetectBall(bool bValue);
	void DrawLine();
	void PressedGrabFire();
	void DrawSweep();
	void ReleasedGrabFire();
	void ReCenter();

	bool bIsLeftDraw = false;
	bool bIsRightDraw = false;
	bool bIsLeftFire = false;
	bool bIsRightFire = false;
	bool bIsReturn = false;
	bool bisSweep = false;
	bool bIsGrabPressed = false;
	FVector leftcurrentPos;
	FVector rightcurrentPos;
	FVector leftstartPos;
	FVector rightstartPos;
	FVector handPos;
	FVector ballLoc;


	UPROPERTY(EditAnywhere)
		float fireDistance = 100.0f;
	UPROPERTY(EditAnywhere)
		float goalDir = 5000.0f;
	UPROPERTY(EditAnywhere)
		float speed = 8000;
	float axis = 0;
	float Axis = 0;
	float currtime;
	float delta;
	float Deltatime = 0;
	int32 state_L = 0;
	int32 state_R = 0;
};
