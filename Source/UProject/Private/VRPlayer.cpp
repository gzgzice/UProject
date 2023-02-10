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

	originPos = leftHand->GetRelativeLocation();
	UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), originPos.X, originPos.Y, originPos.Z);
}

// Called every frame
void AVRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	handPos = leftHand->GetRelativeLocation();

	if (bIsDraw)
	{
		DrawLocationLine();
	}

	if (bIsFire)
	{
		FireHand(DeltaTime);

	//	//FVector handForward = FRotationMatrix(rightHand->GetComponentRotation()).GetUnitAxis(EAxis::Y);
	//	//FVector handUp = FRotationMatrix(rightHand->GetComponentRotation()).GetUnitAxis(EAxis::X);
	//	//FVector p0 = rightHand->GetComponentLocation();
	//	//FVector d = rightHand->GetComponentLocation() + handForward + handUp;
	//	//FVector vt = d  * axis * speed * DeltaTime;
	//	//FHitResult hitInfo;

	//	//rightHand->SetWorldLocation(p0 + vt);

	//	//FVector p = p0 + vt;

	//	//UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), p.X, p.Y, p.Z);

	//	//DrawDebugLine(GetWorld(), p0, p, FColor::Green, false, -1, 0, 5);
	}

	if (bIsReturn)
	{
		HandReturnMove(DeltaTime);
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
			SetActorLocation(actor->GetActorLocation() + GetActorUpVector() * 90 );
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
	FVector pos1 = GetActorUpVector() * -50;
	FVector pos2 = GetActorForwardVector() * 500;
	FVector endLoc = startLoc + pos1 + pos2;

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

	//if (bIsFire)
	//{
	//	FireHand();
	//}
	//FVector rightHandLoc = rightMotionController->GetComponentLocation();
	//FVector dir = originPos - rightHandLoc;
	//if (dir.Length() > goalDir)
	//{
	//	rightMotionController->SetRelativeLocation(originPos);
	//}
}

void AVRPlayer::FireHand(float deltatime)
{
	currtime += deltatime;
	//float deltatime = GetWorld()->GetDeltaSeconds();

	FVector handForward = FRotationMatrix(leftHand->GetComponentRotation()).GetUnitAxis(EAxis::Y);
	FVector handUp = FRotationMatrix(leftHand->GetComponentRotation()).GetUnitAxis(EAxis::X) * -2;

	FVector dir = handForward + handUp;

	FVector prediction = leftHand->GetRelativeLocation() + dir * axis * speed * currtime;
	//UE_LOG(LogTemp,Warning,TEXT("TimeSegment : %d & FireHand FVector : %s"), timeSegment,*prediction.ToString())

	leftHand->SetRelativeLocation(prediction);

	FVector p = leftHand->GetRelativeLocation();

	FString msg = FString::Printf(TEXT("%f"), p.X, p.Y, p.Z);
	rightLog->SetText(FText::FromString(msg));

	FVector dirrection = originPos - leftHand->GetRelativeLocation();
	if (dirrection.Length() > goalDir)
	{
		bIsFire = false;
		FVector returnPos = FMath::Lerp(handPos, originPos, currtime);
		leftHand->SetRelativeLocation(originPos);
		UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), returnPos.X, returnPos.Y, returnPos.Z);
	}

	//	FVector Return = FMath::Lerp(prediction, prediction, deltatime);
}

void AVRPlayer::ReturnRightHand()
{
	UE_LOG(LogTemp,Warning,TEXT("returnRigth!!!!!"))
	bIsFire = false;
	
	if (!bIsFire)
	{
		bIsReturn = true;
		//leftHand->SetRelativeLocation(originPos);
	}
}

void AVRPlayer::HandReturnMove(float deltatime)
{
	cTime += deltatime;

	FVector handForward = FRotationMatrix(leftHand->GetComponentRotation()).GetUnitAxis(EAxis::Y) * -1;
	FVector handUp = FRotationMatrix(leftHand->GetComponentRotation()).GetUnitAxis(EAxis::X) * 2;

	FVector dir = handForward + handUp;

	FVector p = leftHand->GetRelativeLocation() + dir * speed * cTime;
	leftHand->SetRelativeLocation(p);
	UE_LOG(LogTemp, Error, TEXT("%f,%f,%f"), p.X, p.Y, p.Z);

	FVector direction = originPos - leftHand->GetRelativeLocation();

	if (direction.Length() > goalDir)
	{
		bIsReturn = false;
		leftHand->SetRelativeLocation(originPos);
	}
}

//void AVRPlayer::MoveAction(float deltatime)
//{
//
//}
