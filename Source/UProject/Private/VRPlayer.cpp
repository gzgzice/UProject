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


// Sets default values
AVRPlayer::AVRPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Cam->SetupAttachment(RootComponent);

	headMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	headMesh->SetupAttachment(Cam);
	headMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	rightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("rightController"));
	rightMotionController->SetupAttachment(RootComponent);
	rightMotionController->MotionSource = "Right";

	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("rightHand"));
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

	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>("leftHand");
	leftHand->SetupAttachment(leftMotionController);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHand->SetRelativeRotation(FRotator(-25.0f, 180.0f, 90.0f));

	leftLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Left Log Text"));
	leftLog->SetupAttachment(leftMotionController);
	leftLog->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	leftLog->SetTextRenderColor(FColor::Yellow);
	leftLog->SetHorizontalAlignment(EHTA_Center);
	leftLog->SetVerticalAlignment(EVRTA_TextBottom);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;

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

	startPos = leftHand->GetComponentLocation();
}

// Called every frame
void AVRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentPos = leftHand->GetComponentLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), currentPos.X, currentPos.Y, currentPos.Z);

	if (bIsDraw)
	{
		DrawLocationLine();
	}

	if (bIsFire)
	{
		FireHand(DeltaTime);
	}
	else
	{
		ReturnMove(DeltaTime);
	}
}

// Called to bind functionality to input
void AVRPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(leftActionX, ETriggerEvent::Started, this, &AVRPlayer::OnLeftActionX);
		enhancedInputComponent->BindAction(leftActionX, ETriggerEvent::Completed, this, &AVRPlayer::ReleaseActionX);
		enhancedInputComponent->BindAction(RightThumbStick, ETriggerEvent::Triggered, this, &AVRPlayer::RotateAxis);
		enhancedInputComponent->BindAction(rightTrigger, ETriggerEvent::Triggered, this, &AVRPlayer::FireRightHand);
		enhancedInputComponent->BindAction(rightTrigger, ETriggerEvent::Completed, this, &AVRPlayer::ReturnRightHand);
	}
}

void AVRPlayer::OnLeftActionX()
{
	bIsDraw = true;

	FString msg = FString::Printf(TEXT("ActionX"));
	leftLog->SetText(FText::FromString(msg));

	FVector startLoc = GetActorLocation();
	FVector pos = GetActorForwardVector() * 500;
	FVector endLoc = startLoc + pos;
	FHitResult hitInfo;

	//bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECC_Visibility);
	bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startLoc, endLoc, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeSphere(fireDistance));

	if (bHit)
	{
		AActor* actor = hitInfo.GetActor();
		leftLog->SetText(FText::FromString(hitInfo.GetActor()->GetName()));
		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), actor)
		if (actor->GetName().Contains(TEXT("Point")))
		{
			SetActorLocation(actor->GetActorLocation() + GetActorUpVector() * 90 );
			startPos = leftHand->GetComponentLocation();
		}
	}

}

void AVRPlayer::ReleaseActionX()
{
	bIsDraw = false;
}

void AVRPlayer::DrawLocationLine()
{
	FVector startLoc = GetActorLocation();
	FVector pos = GetActorForwardVector() * 500;
	FVector endLoc = startLoc + pos;

	DrawDebugSphere(GetWorld(), endLoc,
	fireDistance, 30, FColor::Cyan, false, -1, 0, 1);
}

void AVRPlayer::RotateAxis(const FInputActionValue& value)
{
	float Axis = value.Get<float>();

	AddControllerYawInput(Axis);
}

void AVRPlayer::FireRightHand(const FInputActionValue& value)
{
	UE_LOG(LogTemp,Warning,TEXT("Fire Right Hand!!!!!"))
	axis = value.Get<float>();
	FString msg = FString::Printf(TEXT("%f"), axis);
	rightLog->SetText(FText::FromString(msg));

	bIsFire = true;
	//currentPos = leftHand->GetComponentLocation() + leftHand->GetForwardVector() * 100 + delta;
}

void AVRPlayer::FireHand(float deltatime)
{
	currtime += GetWorld()->GetDeltaSeconds();

	FVector handForward = FRotationMatrix(leftHand->GetComponentRotation()).GetUnitAxis(EAxis::Y);
	FVector handUp = FRotationMatrix(leftHand->GetComponentRotation()).GetUnitAxis(EAxis::X) * -2;

	FVector dir = handForward + handUp;

	FVector prediction = leftHand->GetComponentLocation() + dir * axis * speed * deltatime;
	//UE_LOG(LogTemp,Warning,TEXT("TimeSegment : %d & FireHand FVector : %s"), timeSegment,*prediction.ToString())

	leftHand->SetWorldLocation(prediction);

	FVector p = leftHand->GetComponentLocation();

	//FVector direction = startPos - leftHand->GetComponentLocation();
	//if (direction.Length() > goalDir)
	//{
	//	ReturnRightHand();
	//}
}

void AVRPlayer::ReturnRightHand()
{
	UE_LOG(LogTemp,Warning,TEXT("returnRigth!!!!!"))

	bIsFire = false;
}

void AVRPlayer::ReturnMove(float deltatime)
{
	UE_LOG(LogTemp, Error, TEXT("deltaTime = %f"), deltatime)
	FVector returnPos = FMath::Lerp(currentPos, startPos, deltatime * 5);
	leftHand->SetWorldLocation(returnPos);
	//UE_LOG(LogTemp, Warning, TEXT("returnPos = %f,%f,%f"), returnPos.X, returnPos.Y, returnPos.Z);
}
