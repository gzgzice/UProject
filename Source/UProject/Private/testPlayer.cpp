// Fill out your copyright notice in the Description page of Project Settings.


#include "testPlayer.h"
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
AtestPlayer::AtestPlayer()
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

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AtestPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackOrigin.GetValue());

	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());

	subsys->AddMappingContext(myMapping, 0);
}

// Called every frame
void AtestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsDraw)
	{
		DrawLocationLine();
	}
}

// Called to bind functionality to input
void AtestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(leftActionX, ETriggerEvent::Started, this, &AtestPlayer::OnLeftActionX);
		enhancedInputComponent->BindAction(leftActionX, ETriggerEvent::Completed, this, &AtestPlayer::ReleaseActionX);
	}
}

void AtestPlayer::OnLeftActionX()
{
	bIsDraw = true;

	FString msg = FString::Printf(TEXT("ActionX"));
	leftLog->SetText(FText::FromString(msg));

	FVector startLoc = GetActorLocation();
	FVector pos1 = GetActorUpVector() * -50;
	FVector pos2 = GetActorForwardVector() * 500;
	FVector endLoc = startLoc + pos1 + pos2;
	FHitResult hitInfo;

	//bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECC_Visibility);
	bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startLoc, endLoc, FQuat::Identity, ECC_Visibility, 
	FCollisionShape::MakeSphere(fireDistance));

	if (bHit)
	{	
		AActor* actor = hitInfo.GetActor();
		leftLog->SetText(FText::FromString(hitInfo.GetActor()->GetName()));
		if (actor->GetName().Contains(TEXT("Point")))
		{
			SetActorLocation(actor->GetActorLocation());
		}
	}
}

void AtestPlayer::ReleaseActionX()
{
	bIsDraw = false;
}

void AtestPlayer::DrawLocationLine()
{
	FVector startLoc = GetActorLocation();
	FVector pos1 = GetActorUpVector() * -50;
	FVector pos2 = GetActorForwardVector() * 500;
	FVector endLoc = startLoc + pos1 + pos2;

	//DrawDebugLine(GetWorld(), startLoc, endLoc, FColor::Cyan, false, -1, 0, 1);
	DrawDebugSphere(GetWorld(), endLoc, 
	fireDistance, 30, FColor::Cyan, false, -1, 0, 1);
}
