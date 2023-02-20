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



// Sets default values
AVRPlayer::AVRPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	rightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightHand"));
	rightHand->SetupAttachment(rightMotionController);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeRotation(FRotator(25.0f, 0.0f, 90.0f));

	rightLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Right Log Text"));
	rightLog->SetupAttachment(rightMotionController);
	rightLog->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	rightLog->SetTextRenderColor(FColor::Yellow);
	rightLog->SetHorizontalAlignment(EHTA_Center);
	rightLog->SetVerticalAlignment(EVRTA_TextBottom);

	leftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("leftController"));
	leftMotionController->SetupAttachment(RootComponent);
	leftMotionController->MotionSource = "Left";

	leftHand = CreateDefaultSubobject<UStaticMeshComponent>("leftHand");
	leftHand->SetupAttachment(leftMotionController);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHand->SetRelativeRotation(FRotator(-25.0f, 180.0f, 90.0f));

	leftLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Left Log Text"));
	leftLog->SetupAttachment(leftMotionController);
	leftLog->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	leftLog->SetTextRenderColor(FColor::Yellow);
	leftLog->SetHorizontalAlignment(EHTA_Center);
	leftLog->SetVerticalAlignment(EVRTA_TextBottom);

// 	ConstructorHelpers::FClassFinder<UDirectionWidget> tempdirUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/BP_DirectionUI.BP_DirectionUI_C'"));
// 	if (tempdirUI.Succeeded())
// 	{
// 		dirUIFactory = tempdirUI.Class; 
// 	}

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

// 	dirUI = CreateWidget<UDirectionWidget>(GetWorld(), dirUIFactory);
// 	dirUI->AddToViewport();

	ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));

	if (widgetComp != nullptr)
	{
		dirUI = Cast<UDirectionWidget>(widgetComp->GetWidget());
	}
}

// Called every frame
void AVRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	leftcurrentPos = leftHand->GetComponentLocation();
	rightcurrentPos = rightHand->GetComponentLocation();

 	if (bIsRightDraw)
 	{
 		DrawLocationLine(rightMotionController);
 	}
	if (bIsLeftDraw)
	{
		DrawLocationLine(leftMotionController);
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
	//FindAngle();
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
		enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Started, this, &AVRPlayer::OnRightActionA);
		enhancedInputComponent->BindAction(rightInputs[0], ETriggerEvent::Completed, this, &AVRPlayer::ReleaseActionA);
		enhancedInputComponent->BindAction(rightInputs[1], ETriggerEvent::Started, this, &AVRPlayer::RotateRight);
		enhancedInputComponent->BindAction(rightInputs[1], ETriggerEvent::Completed, this, &AVRPlayer::RotateRight);
		enhancedInputComponent->BindAction(rightInputs[2], ETriggerEvent::Triggered, this, &AVRPlayer::FireRightHand);
		enhancedInputComponent->BindAction(rightInputs[2], ETriggerEvent::Completed, this, &AVRPlayer::ReturnRightHand);
	}
}

void AVRPlayer::OnleftActionX()
{
	bIsLeftDraw = true;

	FString msg = FString::Printf(TEXT("ActionX"));
	leftLog->SetText(FText::FromString(msg));

	FVector startLoc = leftMotionController->GetComponentLocation() + leftMotionController->GetForwardVector() * 300;
	FVector pos = leftMotionController->GetForwardVector() * 1000 + leftMotionController->GetUpVector() * -300;
	FVector endLoc = startLoc + pos;
	FHitResult hitInfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startLoc, endLoc, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeSphere(fireDistance), param);

	if (bHit)
	{
		AActor* actor = hitInfo.GetActor();
		leftLog->SetText(FText::FromString(hitInfo.GetActor()->GetName()));
		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
			if (actor->GetName().Contains(TEXT("MovePoint")))
			{
				SetActorLocation(actor->GetActorLocation() + GetActorUpVector() * 91);
				rightstartPos = rightHand->GetComponentLocation();
				leftstartPos = leftHand->GetComponentLocation();
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
	bIsRightDraw = true;

	FString msg = FString::Printf(TEXT("ActionA"));
	rightLog->SetText(FText::FromString(msg));

	////bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECC_Visibility);
	//bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startLoc, endLoc, FQuat::Identity, ECC_Visibility,
	//	FCollisionShape::MakeSphere(fireDistance), param);

	FVector startLoc = rightMotionController->GetComponentLocation() + rightMotionController->GetForwardVector() * 300;
	FVector pos = rightMotionController->GetForwardVector() * 1000 + rightMotionController->GetUpVector() * -300;
	FVector endLoc = startLoc + pos;
	FHitResult hitInfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startLoc, endLoc, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeSphere(fireDistance), param);

	if (bHit)
	{
		AActor* actor = hitInfo.GetActor();
		leftLog->SetText(FText::FromString(hitInfo.GetActor()->GetName()));
		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
			if (actor->GetName().Contains(TEXT("MovePoint")))
			{
				SetActorLocation(actor->GetActorLocation() + GetActorUpVector() * 91);
				rightstartPos = rightHand->GetComponentLocation();
				leftstartPos = leftHand->GetComponentLocation();
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
	FVector pos = motionController->GetForwardVector() * 1000 + motionController->GetUpVector() * -300;
	FVector endLoc = startLoc + pos;

	DrawDebugSphere(GetWorld(), endLoc,
	fireDistance, 30, FColor::Cyan, false, -1, 0, 1);
}

void AVRPlayer::RotateRight()
{
	AddControllerYawInput(90);
	rightHand->SetWorldLocation(rightMotionController->GetComponentLocation());
	leftHand->SetWorldLocation(leftMotionController->GetComponentLocation());
 	rightstartPos = rightMotionController->GetComponentLocation();
 	leftstartPos = leftMotionController->GetComponentLocation();
}

void AVRPlayer::RotateLeft()
{
	AddControllerYawInput(-90);
	rightHand->SetWorldLocation(rightMotionController->GetComponentLocation());
	leftHand->SetWorldLocation(leftMotionController->GetComponentLocation());
 	rightstartPos = rightMotionController->GetComponentLocation();
 	leftstartPos = leftMotionController->GetComponentLocation();
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
	bIsRightFire = true;
}

void AVRPlayer::LeftHandMove(float deltatime)
{
	FVector handForward = FRotationMatrix(leftHand->GetComponentRotation()).GetUnitAxis(EAxis::Y);
	FVector handUp = FRotationMatrix(leftHand->GetComponentRotation()).GetUnitAxis(EAxis::X) * -1;

	FVector dir = handForward + handUp;

	FVector prediction = leftHand->GetComponentLocation() + dir * axis * speed * deltatime;
	//UE_LOG(LogTemp,Warning,TEXT("TimeSegment : %d & FireHand FVector : %s"), timeSegment,*prediction.ToString())

	leftHand->SetWorldLocation(prediction);

  	FVector start = leftHand->GetComponentLocation();
  	FVector end = leftHand->GetComponentLocation();
  
  	DrawDebugSphere(GetWorld(), end,
  		20, 30, FColor::Cyan, false, -1, 0, 1);
  
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
  		ball = Cast<ABall>(actor);
  		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
  
  			UPrimitiveComponent* compHit = hitInfo.GetComponent();
  		if (compHit->IsSimulatingPhysics() == true)
  		{
  			FVector dist = hitInfo.ImpactPoint - GetActorLocation();
  			FVector hitDir = compHit->GetComponentLocation() - hitInfo.ImpactPoint;
  			FVector Loc = dist + hitDir;
  			FVector force = compHit->GetMass() * Loc * 300;
  			compHit->AddForceAtLocation(force, hitInfo.ImpactPoint);
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
	FVector prediction = rightHand->GetComponentLocation() + rightHand->GetForwardVector() * axis * speed * deltatime;
	rightHand->SetWorldLocation(prediction);

	FVector loc = rightHand->GetComponentLocation();

	//UE_LOG(LogTemp, Warning, TEXT("X = %f,Y = %f, Z = %f"), loc.X, loc.Y, loc.Z)

 	FVector start = rightHand->GetComponentLocation();
 	FVector end = rightHand->GetComponentLocation();
 
 	DrawDebugSphere(GetWorld(), end,
 		20, 30, FColor::Cyan, false, -1, 0, 1);
 
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
 		ball = Cast<ABall>(actor);
 		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
 
 			UPrimitiveComponent* compHit = hitInfo.GetComponent();
 		if (compHit->IsSimulatingPhysics() == true)
 		{
 			FVector dist = hitInfo.ImpactPoint - GetActorLocation();
 			FVector hitDir = compHit->GetComponentLocation() - hitInfo.ImpactPoint;
 			FVector Loc = dist + hitDir;
 			FVector force = compHit->GetMass() * Loc * 300;
 			compHit->AddForceAtLocation(force, hitInfo.ImpactPoint);
 		}
 	}
 	if (bHitDome)
 	{
 		AActor* actor = hitDome.GetActor();
 		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
 			bIsRightFire = false;
 	}

	//DetectObject(rightHand, bIsRightFire);
}

void AVRPlayer::ReturnLeftHand()
{
	bIsLeftFire = false;
}

void AVRPlayer::ReturnRightHand()
{
	bIsRightFire = false;
}

void AVRPlayer::ReturnMove(float deltatime, FVector startPos, FVector currPos, UStaticMeshComponent* handmesh)
{
	FVector returnPos = FMath::Lerp(currPos, startPos, deltatime * 30);
	handmesh->SetWorldLocation(returnPos);
}

void AVRPlayer::FindAngle()
{	
// 	FVector player = FVector (GetActorLocation().X, GetActorLocation().Y, 0);
// 	FVector target = FVector (ball->GetActorLocation().X, ball->GetActorLocation().Y, 0);
// 	FVector dir = target - player;
// 	dir.Normalize();
// 	float dot = FVector::DotProduct(GetActorForwardVector(), dir);
// 	float acos = FMath::Acos(dot);
// 	float angle = FMath::RadiansToDegrees(acos);
// 	FVector cross = FVector::CrossProduct(GetActorForwardVector(), dir);
// 	float turnAngle = 0;
// 
// 	if (cross.Z < 0)
// 	{
// 		turnAngle = -angle;
// 	}
// 	else if(cross.Z > 0)
// 	{
// 		turnAngle = angle;
// 	}
// 	dirUI->ArrowRotation(turnAngle);

//  	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorForwardVector(), ball->GetActorLocation());
//  	float angle = Rotator.Yaw - GetControlRotation().Yaw;
//  	dirUI->ArrowRotation(angle);

	FVector dir = ball->GetActorLocation() - GetActorLocation();
	dir.Normalize();
	FRotator rot = UKismetMathLibrary::MakeRotFromX(dir);
	float angle = rot.Yaw;
	dirUI->ArrowRotation(angle);
}

// void AVRPlayer::DetectObject(USkeletalMeshComponent* handmesh, bool varName)
// {
// 	FVector start = handmesh->GetComponentLocation();
// 	FVector end = handmesh->GetComponentLocation();
// 
// 	DrawDebugSphere(GetWorld(), end,
// 		20, 30, FColor::Cyan, false, -1, 0, 1);
// 
// 	FHitResult hitInfo;
// 	FCollisionQueryParams param;
// 	param.AddIgnoredActor(this);
// 
// 	FHitResult hitDome;
// 	FCollisionQueryParams params;
// 	params.AddIgnoredActor(this);
// 
// 	bool bHitball = GetWorld()->SweepSingleByChannel(hitInfo, start, end, FQuat::Identity, ECC_Visibility,
// 		FCollisionShape::MakeSphere(20), param);
// 	bool bHitDome = GetWorld()->SweepSingleByObjectType(hitDome, start, end, FQuat::Identity,
// 		ECC_GameTraceChannel8, FCollisionShape::MakeSphere(20), params);
// 
// 	if (bHitball)
// 	{
// 		AActor* actor = hitInfo.GetActor();
// 		ball = Cast<ABall>(actor);
// 		//UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
// 
// 			UPrimitiveComponent* compHit = hitInfo.GetComponent();
// 		if (compHit->IsSimulatingPhysics() == true)
// 		{
// 			FVector dist = hitInfo.ImpactPoint - GetActorLocation();
// 			FVector hitDir = compHit->GetComponentLocation() - hitInfo.ImpactPoint;
// 			FVector Loc = dist + hitDir;
// 			FVector force = compHit->GetMass() * Loc * 300;
// 			compHit->AddForceAtLocation(force, hitInfo.ImpactPoint);
// 		}
// 	}
// 	if (bHitDome)
// 	{
// 		AActor* actor = hitDome.GetActor();
// 		UE_LOG(LogTemp, Warning, TEXT("hirInfo : %s"), *actor->GetName())
// 		varName = false;
// 	}
//}
