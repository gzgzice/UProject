// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayer.h"
#include <Camera/CameraComponent.h>
#include <MotionControllerComponent.h>
#include <Components/StaticMeshComponent.h>
#include "HeadMountedDisplayFunctionLibrary.h"
#include <Components/TextRenderComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Ball.h"
#include <Kismet/GameplayStatics.h>
#include "UGameModeBase.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include "DirectionWidget.h"
#include "RedGoalPost.h"
#include <Particles/ParticleSystem.h>
#include "PlayerEffectActor.h"
#include <Particles/ParticleSystemComponent.h>
#include <Components/CapsuleComponent.h>
#include <UMG/Public/Components/WidgetInteractionComponent.h>



// Sets default values
AVRPlayer::AVRPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.5, 1.5, 1.0));
	GetCapsuleComponent()->SetCapsuleRadius(80);

	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Cam->SetupAttachment(RootComponent);

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	widgetComp->SetupAttachment(Cam);
	widgetComp->SetRelativeLocation(FVector(500, 0, 0));
	widgetComp->SetRelativeRotation(FRotator(0, 180, 0));

	headMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	headMesh->SetupAttachment(Cam);
	headMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	rightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("rightController"));
	rightMotionController->SetupAttachment(RootComponent);
	rightMotionController->MotionSource = "Right";
	rightMotionController->SetRelativeLocation(FVector(23, 16,-25));

	rightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightHand"));
	rightHand->SetupAttachment(rightMotionController);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeScale3D(FVector(0.2));

	leftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("leftController"));
	leftMotionController->SetupAttachment(RootComponent);
	leftMotionController->MotionSource = "Left";
	leftMotionController->SetRelativeLocation(FVector(23, -23, -25));

	leftHand = CreateDefaultSubobject<UStaticMeshComponent>("leftHand");
	leftHand->SetupAttachment(leftMotionController);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHand->SetRelativeScale3D(FVector(0.2));

	Effect_L = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect_L"));
	Effect_L->SetupAttachment(leftHand);
	Effect_L->SetRelativeLocation(FVector(-150,0,0));
	Effect_L->SetRelativeRotation(FRotator(90,0,0));
	Effect_L->SetVisibility(false);

	Effect_R = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect_R"));
	Effect_R->SetupAttachment(rightHand);
	Effect_R->SetRelativeLocation(FVector(-150, 0, 0));
	Effect_R->SetRelativeRotation(FRotator(90, 0, 0));
	Effect_R->SetVisibility(false);

	widgetPointer_right = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Right Widget Pointer"));
	widgetPointer_right->SetupAttachment(rightMotionController);
	widgetPointer_right->InteractionDistance = 1000.0f;
	widgetPointer_right->bShowDebug = false;
	widgetPointer_right->DebugColor = FColor::Yellow;

	bUseControllerRotationYaw = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AVRPlayer::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackOrigin.GetValue());

	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

	subsys->AddMappingContext(myMapping, 0);

	leftstartPos = leftMotionController->GetComponentLocation();
	rightstartPos = rightMotionController->GetComponentLocation();

	ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));

	if (widgetComp != nullptr)
	{
		dirUI = Cast<UDirectionWidget>(widgetComp->GetWidget());
	}

	orginPos = GetActorLocation();

	RedGoalPost = Cast<ARedGoalPost>(UGameplayStatics::GetActorOfClass(GetWorld(), ARedGoalPost::StaticClass()));
}

// Called every frame
void AVRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	delta += DeltaTime;

	leftcurrentPos = leftHand->GetComponentLocation();
	rightcurrentPos = rightHand->GetComponentLocation();

  //	if (bIsRightDraw)
  //	{
  //		DrawLocationLine(rightMotionController);
  //	}
 
 	//if (bIsLeftDraw)
 	//{
 	//	DrawLocationLine(leftMotionController);
 	//}

	if (bisSweep)
	{
		DrawSweep();
	}

	if (bIsLeftFire)
	{
		LeftHandMove(DeltaTime);
	}
	else
	{
		ReturnMove(DeltaTime, leftstartPos, leftcurrentPos, leftHand);
	}	
	
	if (bIsRightFire)
	{
		RightHandMove(DeltaTime);
	}
	else
	{
		ReturnMove(DeltaTime, rightstartPos, rightcurrentPos, rightHand);
	}
	
	FindAngle();
}

// Called to bind functionality to input
void AVRPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Started, this, &AVRPlayer::OnleftActionX);
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Completed, this, &AVRPlayer::ReleaseActionX);
		enhancedInputComponent->BindAction(leftInputs[1], ETriggerEvent::Started, this, &AVRPlayer::RotateLeft);
		enhancedInputComponent->BindAction(leftInputs[1], ETriggerEvent::Completed, this, &AVRPlayer::RotateLeft);
		enhancedInputComponent->BindAction(leftInputs[2], ETriggerEvent::Triggered, this, &AVRPlayer::FireLeftHand);
		enhancedInputComponent->BindAction(leftInputs[2], ETriggerEvent::Completed, this, &AVRPlayer::ReturnLeftHand);
		enhancedInputComponent->BindAction(leftInputs[3], ETriggerEvent::Started, this, &AVRPlayer::ReCenter);
		enhancedInputComponent->BindAction(leftInputs[3], ETriggerEvent::Completed, this, &AVRPlayer::ReCenter);
		enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Started, this, &AVRPlayer::OnRightActionA);
		enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &AVRPlayer::ReleaseActionA);
		enhancedInputComponent->BindAction(rightInputs[1], ETriggerEvent::Started, this, &AVRPlayer::RotateRight);
		enhancedInputComponent->BindAction(rightInputs[1], ETriggerEvent::Completed, this, &AVRPlayer::RotateRight);
		enhancedInputComponent->BindAction(rightInputs[2], ETriggerEvent::Triggered, this, &AVRPlayer::FireRightHand);
		enhancedInputComponent->BindAction(rightInputs[2], ETriggerEvent::Completed, this, &AVRPlayer::ReturnRightHand);
		enhancedInputComponent->BindAction(rightInputs[3], ETriggerEvent::Started, this, &AVRPlayer::PressedGrabFire);
		enhancedInputComponent->BindAction(rightInputs[3], ETriggerEvent::Completed, this, &AVRPlayer::ReleasedGrabFire);
	}
}

void AVRPlayer::OnleftActionX()
{
	UGameplayStatics::PlaySound2D(GetWorld(), moveSound);

	bIsLeftDraw = true;

	FVector startLoc = leftMotionController->GetComponentLocation() + leftMotionController->GetForwardVector() * 300;
	FVector pos = leftMotionController->GetForwardVector() * 900 + leftMotionController->GetUpVector() * -500;
	FVector endLoc = startLoc + pos;
	FHitResult hitInfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startLoc, endLoc, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeSphere(fireDistance), param);

	if (bHit)
	{
		AActor* actor = hitInfo.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
			if (actor->GetName().Contains(TEXT("MovePoint")))
			{
				SetActorLocation(actor->GetActorLocation() + GetActorUpVector() * 91);
				rightstartPos = rightMotionController->GetComponentLocation();
				leftstartPos = leftMotionController->GetComponentLocation();
//  				rightHand->SetWorldLocation(rightstartPos);
//  				leftHand->SetWorldLocation(leftstartPos);
			}
	}
	else return;
}

void AVRPlayer::ReleaseActionX()
{
	bIsLeftDraw = false;
}

void AVRPlayer::OnRightActionA()
{
	UGameplayStatics::PlaySound2D(GetWorld(), moveSound);
	bIsRightDraw = true;

	////bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECC_Visibility);
	//bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startLoc, endLoc, FQuat::Identity, ECC_Visibility,
	//	FCollisionShape::MakeSphere(fireDistance), param);

	FVector startLoc = rightMotionController->GetComponentLocation() + rightMotionController->GetForwardVector() * 300;
	FVector pos = rightMotionController->GetForwardVector() * 900 + rightMotionController->GetUpVector() * -500;
	FVector endLoc = startLoc + pos;
	FHitResult hitInfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startLoc, endLoc, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeSphere(fireDistance), param);

	if (bHit)
	{
		AActor* actor = hitInfo.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
			if (actor->GetName().Contains(TEXT("MovePoint")))
			{
				SetActorLocation(actor->GetActorLocation() + GetActorUpVector() * 91);
				rightstartPos = rightMotionController->GetComponentLocation();
				leftstartPos = leftMotionController->GetComponentLocation();
//  				rightHand->SetWorldLocation(rightstartPos);
//  				leftHand->SetWorldLocation(leftstartPos);
			}
	}
	else return;

}

void AVRPlayer::ReleaseActionA()
{
	bIsRightDraw = false;
}

void AVRPlayer::DrawLocationLine(UMotionControllerComponent* motionController)
{
	FVector startLoc = motionController->GetComponentLocation() + motionController->GetForwardVector() * 300;
	FVector pos = motionController->GetForwardVector() * 900 + motionController->GetUpVector() * -500;
	FVector endLoc = startLoc + pos;

	DrawDebugSphere(GetWorld(), endLoc,
	fireDistance, 30, FColor::Cyan, false, -1, 0, 1);
}

void AVRPlayer::RotateRight()
{
// 	FRotator curRot = GetActorRotation();
// 	FRotator newRot = FRotator(0, curRot.Yaw + -90, 0);
// 	SetActorRotation(newRot);

	//UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(90);
	rightHand->SetWorldLocation(rightcurrentPos);
	leftHand->SetWorldLocation(leftcurrentPos);
	rightstartPos = rightMotionController->GetComponentLocation();
	leftstartPos = leftMotionController->GetComponentLocation();
	AddControllerYawInput(90);
}

void AVRPlayer::RotateLeft()
{
// 	FRotator curRot = GetActorRotation();
// 	FRotator newRot = FRotator(0, curRot.Yaw + 90, 0);
// 	SetActorRotation(newRot);

	rightHand->SetWorldLocation(rightcurrentPos);
	leftHand->SetWorldLocation(leftcurrentPos);
	rightstartPos = rightMotionController->GetComponentLocation();
   	leftstartPos = leftMotionController->GetComponentLocation();
 	AddControllerYawInput(-90);
}

void AVRPlayer::FireLeftHand(const FInputActionValue& value)
{
	axis = value.Get<float>();
	bIsLeftFire = true;
}

void AVRPlayer::FireRightHand(const struct FInputActionValue& value)
{
	axis = value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT("rightHand"))
	bIsRightFire = true;;
}

void AVRPlayer::LeftHandMove(float deltatime)
{
	FVector prediction = leftHand->GetComponentLocation() + leftHand->GetForwardVector()
	 * axis * speed * deltatime;
	//UE_LOG(LogTemp,Warning,TEXT("TimeSegment : %d & FireHand FVector : %s"), timeSegment,*prediction.ToString())

	leftHand->SetWorldLocation(prediction);

	if (state_L == 0)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
		state_L++;
	}

	Effect_L->SetVisibility(true);

  	FVector start = leftHand->GetComponentLocation();
  	FVector end = leftHand->GetComponentLocation();
  
//   	DrawDebugSphere(GetWorld(), end,
//   		20, 30, FColor::Cyan, false, -1, 0, 1);
  
  	FHitResult hitInfo;
  	FCollisionQueryParams param;
  	param.AddIgnoredActor(this);
  
  	FHitResult hitDome;
  	FCollisionQueryParams params;
  	params.AddIgnoredActor(this);
  
  	bool bHitball = GetWorld()->SweepSingleByChannel(hitInfo, start, end, FQuat::Identity, ECC_Visibility,
  		FCollisionShape::MakeSphere(20), param);
  	bool bHitDome = GetWorld()->SweepSingleByObjectType(hitDome, start, end, FQuat::Identity,
  		ECC_GameTraceChannel8, FCollisionShape::MakeSphere(20), params);
  
  	if (bHitball)
  	{	
  		AActor* actor = hitInfo.GetActor();
		ABall* hitActor = Cast<ABall>(actor);
		if (hitActor != nullptr)
		{
			ball = hitActor;
		}
  		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
  
  			UPrimitiveComponent* compHit = hitInfo.GetComponent();
  		if (compHit->IsSimulatingPhysics() == true)
  		{
  			FVector dist = hitInfo.ImpactPoint - GetActorLocation();
  			FVector hitDir = compHit->GetComponentLocation() - hitInfo.ImpactPoint;
  			FVector Loc = dist + hitDir;
  			FVector force = compHit->GetMass() * Loc * 150;
  			compHit->AddForceAtLocation(force, hitInfo.ImpactPoint);
			bIsLeftFire = false;
			UGameplayStatics::PlaySound2D(GetWorld(), hitSound);
  		}
  	}
  	if (bHitDome)
  	{
  		AActor* actor = hitDome.GetActor();
  		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
  		bIsLeftFire = false;
  	}
}

void AVRPlayer::RightHandMove(float deltatime)
{
	//FVector down = rightHand->GetUpVector() * -1;

	FVector prediction = rightHand->GetComponentLocation() + rightHand->GetForwardVector() 
	 * axis * speed * deltatime;

	rightHand->SetWorldLocation(prediction);

	if (state_R == 0)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
		state_R++;
	}

	Effect_R->SetVisibility(true);

 	FVector start = rightHand->GetComponentLocation();
 	FVector end = rightHand->GetComponentLocation();
 
//  	DrawDebugSphere(GetWorld(), end,
//  		20, 30, FColor::Cyan, false, -1, 0, 1);
 
 	FHitResult hitInfo;
 	FCollisionQueryParams param;
 	param.AddIgnoredActor(this);
 
 	FHitResult hitDome;
 	FCollisionQueryParams params;
 	params.AddIgnoredActor(this);
 
 	bool bHitball = GetWorld()->SweepSingleByChannel(hitInfo, start, end, FQuat::Identity, ECC_Visibility,
 		FCollisionShape::MakeSphere(20), param);
 	bool bHitDome = GetWorld()->SweepSingleByObjectType(hitDome, start, end, FQuat::Identity,
 		ECC_GameTraceChannel8, FCollisionShape::MakeSphere(20), params);
 
 	if (bHitball)
 	{
 		AActor* actor = hitInfo.GetActor();
 		ABall* hitActor = Cast<ABall>(actor);
		if (hitActor != nullptr)
		{
			ball = hitActor;
		}
 		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
 
 			UPrimitiveComponent* compHit = hitInfo.GetComponent();
 		if (compHit->IsSimulatingPhysics() == true)
 		{
 			FVector dist = hitInfo.ImpactPoint - GetActorLocation();
 			FVector hitDir = compHit->GetComponentLocation() - hitInfo.ImpactPoint;
 			FVector Loc = dist + hitDir;
 			FVector force = compHit->GetMass() * Loc * 150;
 			compHit->AddForceAtLocation(force, hitInfo.ImpactPoint);
			bIsRightFire = false;
			UGameplayStatics::PlaySound2D(GetWorld(), hitSound);
 		}
 	}
 	if (bHitDome)
 	{
 		AActor* actor = hitDome.GetActor();
 		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
 		bIsRightFire = false;
 	}
}

void AVRPlayer::ReturnLeftHand()
{
	bIsLeftFire = false;
	Effect_L->SetVisibility(false);
	state_L = 0;
}

void AVRPlayer::ReturnRightHand()
{
	bIsRightFire = false;
	Effect_R->SetVisibility(false);
	state_R = 0;
}

void AVRPlayer::ReturnMove(float deltatime, FVector startPos, FVector currPos, UStaticMeshComponent* handmesh)
{
	FVector returnPos = FMath::Lerp(currPos, startPos, deltatime * 30);
	handmesh->SetWorldLocation(returnPos);
}

void AVRPlayer::FindAngle()
{	
 	FVector player = FVector(GetActorLocation().X, GetActorLocation().Y, 0);
 	FVector target = FVector(ball->GetActorLocation().X, ball->GetActorLocation().Y, 0);
 	FVector dir = target - player;
 	dir.Normalize();
 	float dot = FVector::DotProduct(GetActorForwardVector(), dir);
 	float acos = FMath::Acos(dot);
 	float angle = FMath::RadiansToDegrees(acos);
 	FVector cross = FVector::CrossProduct(GetActorForwardVector(), dir);
 	float turnAngle = 0;
 
 	if (cross.Z < 0)
 	{
 		turnAngle = -angle;
 	}
 	else if (cross.Z > 0)
 	{
 		turnAngle = angle;
 	}
 	dirUI->ArrowRotation(turnAngle);

//    	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorForwardVector(), ball->GetActorLocation());
//    	float angle = Rotator.Yaw - GetControlRotation().Yaw;
//    	dirUI->ArrowRotation(angle);

//  	FVector dir = ball->GetActorLocation() - GetActorLocation();
//  	dir.Normalize();
//  	FRotator rot = UKismetMathLibrary::MakeRotFromX(dir);
//  	float angle = rot.Yaw;
//  	dirUI->ArrowRotation(angle);
}

void AVRPlayer::DetectBall(bool bValue)
{
	bIsGrabPressed = bValue;
}

void AVRPlayer::PressedGrabFire()
{
	if (bIsGrabPressed)
	{
		bisSweep = true;
		FVector p = rightHand->GetComponentLocation() + rightHand->GetForwardVector() * 5000 * delta;
		//FVector p = ball->GetActorLocation();
		UGameplayStatics::PlaySound2D(GetWorld(), powerhitSound);
		rightHand->SetWorldLocation(p);
		widgetPointer_right->bShowDebug = false;
	}
	else
	{
		bisSweep = false;
	}


}

void AVRPlayer::DrawSweep()
{
	FVector start = rightHand->GetComponentLocation();
	FVector end = rightHand->GetComponentLocation();

// 	DrawDebugSphere(GetWorld(), end,
// 		20, 30, FColor::Cyan, false, -1, 0, 1);

	FHitResult hitInfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);

	FHitResult hitDome;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHitball = GetWorld()->SweepSingleByChannel(hitInfo, start, end, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeSphere(20), param);
	bool bHitDome = GetWorld()->SweepSingleByObjectType(hitDome, start, end, FQuat::Identity,
		ECC_GameTraceChannel8, FCollisionShape::MakeSphere(20), params);

	if (bHitball)
	{
		AActor* actor = hitInfo.GetActor();
		ABall* hitActor = Cast<ABall>(actor);
		if (hitActor != nullptr)
		{
			ball = hitActor;
		}
		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())

			UPrimitiveComponent* compHit = hitInfo.GetComponent();
		if (compHit->IsSimulatingPhysics() == true)
		{
			FVector dist = hitInfo.ImpactPoint - GetActorLocation();
			FVector Loc = RedGoalPost->GetActorLocation() + FVector(0,700,0);
			FVector force = compHit->GetMass() * Loc * 300;
			compHit->AddForceAtLocation(force, hitInfo.ImpactPoint);
			bIsRightFire = false;
			UGameplayStatics::PlaySound2D(GetWorld(), explosionSound);
		}
	}
	if (bHitDome)
	{
		AActor* actor = hitDome.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
		bIsRightFire = false;
	}
}

void AVRPlayer::ReleasedGrabFire()
{
	bIsGrabPressed = false;
}

void AVRPlayer::ResetPos()
{
	SetActorLocation(orginPos+FVector(0,0,0));
	leftstartPos = leftMotionController->GetComponentLocation();
	rightstartPos = rightMotionController->GetComponentLocation();
	rightHand->SetWorldLocation(rightstartPos);
	leftHand->SetWorldLocation(leftstartPos);
}

void AVRPlayer::ReCenter()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}