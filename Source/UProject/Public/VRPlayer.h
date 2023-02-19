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

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UTextRenderComponent* leftLog;

	UPROPERTY(EditAnywhere, Category = "VR_Settings | Components")
		class UTextRenderComponent* rightLog;

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
		TSubclassOf<class UDirectionWidget> dirUIFactory;

private:
	void OnleftActionX();
	void ReleaseActionX();
	void OnRightActionA();
	void ReleaseActionA();
	void RotateRight();
	void RotateLeft();
	void DrawLocationLine(UMotionControllerComponent* motionController);
	void FireLeftHand(const struct FInputActionValue& value);
	void FireRightHand(const struct FInputActionValue& value);
	void LeftHandMove(float deltatime);
	void RightHandMove(float deltatime);
	void ReturnLeftHand();
	void ReturnRightHand();
	void ReturnMove(float deltatime, FVector startPos, FVector currPos, UStaticMeshComponent* handmesh);
	void FindAngle();
	//void DetectObject(USkeletalMeshComponent* handmesh, bool varName);
	//void ChangeHandLocation(float deltatime);

	bool bIsLeftDraw = false;
	bool bIsRightDraw = false;
	bool bIsLeftFire = false;
	bool bIsRightFire = false;
	bool bIsReturn = false;
	FVector leftcurrentPos;
	FVector rightcurrentPos;
	FVector leftstartPos;
	FVector rightstartPos;
	FVector handPos;
	FVector ballLoc;


	UPROPERTY(EditAnywhere)
	float fireDistance = 200.0f;
	UPROPERTY(EditAnywhere)
	float goalDir = 5000.0f;
	UPROPERTY(EditAnywhere)
	float speed = 3000;
	float axis = 0;
	float currtime;
};
