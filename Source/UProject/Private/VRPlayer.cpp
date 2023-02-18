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
#include "ScoreWidget.h"
#include "ScoreWidgetActor.h"
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

	ConstructorHelpers::FClassFinder<UDirectionWidget> tempdirUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/BP_DirectionUI.BP_DirectionUI_C'"));
	if (tempdirUI.Succeeded())
	{
		dirUIFactory = tempdirUI.Class; 
	}

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	ScoreWidgetActor = Cast<AScoreWidgetActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AScoreWidgetActor::StaticClass()));
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

	//ScoreWidgetActor->scoreWG->InitWidget();
	scoreUI = Cast<UScoreWidget>(ScoreWidgetActor->scoreWG->GetUserWidgetObject());

	dirUI = CreateWidget<UDirectionWidget>(GetWorld(), dirUIFactory);

	ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
	ballLoc = ball->GetActorLocation();
}

// Called every frame
void AVRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FindAngle();

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
		enhancedInputComponent->BindAction(RightThumbStick, ETriggerEvent::Started, this, &AVRPlayer::RotateRightAxis);
		enhancedInputComponent->BindAction(RightThumbStick, ETriggerEvent::Completed, this, &AVRPlayer::RotateRightAxis);
		enhancedInputComponent->BindAction(rightTrigger, ETriggerEvent::Triggered, this, &AVRPlayer::FireRightHand);
		enhancedInputComponent->BindAction(rightTrigger, ETriggerEvent::Completed, this, &AVRPlayer::ReturnRightHand);
	}
}

void AVRPlayer::OnLeftActionX()
{
	bIsDraw = true;

	scoreUI->UpdateRedScoreUI(1);

	FString msg = FString::Printf(TEXT("ActionX"));
	leftLog->SetText(FText::FromString(msg));

	////bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECC_Visibility);
	//bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, startLoc, endLoc, FQuat::Identity, ECC_Visibility,
	//	FCollisionShape::MakeSphere(fireDistance), param);

	FVector startLoc = rightMotionController->GetComponentLocation() + rightMotionController->GetForwardVector() * 300;
	FVector pos = rightMotionController->GetForwardVector() * 1000;
	FVector pos1 = rightMotionController->GetUpVector() * 100;
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
				startPos = leftMotionController->GetComponentLocation();
			}
	}
	else return;

}

void AVRPlayer::ReleaseActionX()
{
	bIsDraw = false;
}

void AVRPlayer::DrawLocationLine()
{
	FVector startLoc = rightMotionController->GetComponentLocation() + rightMotionController->GetForwardVector() * 300;
	FVector pos = rightMotionController->GetForwardVector() * 1000;
	FVector pos1 = rightMotionController->GetUpVector() * 100;
	FVector endLoc = startLoc + pos;

	DrawDebugSphere(GetWorld(), endLoc,
	fireDistance, 30, FColor::Cyan, false, -1, 0, 1);

	//FVector startLoc = GetActorLocation() + GetActorForwardVector() * 300;
	//FVector pos = GetActorForwardVector() * 500;
	////FVector pos1 = GetActorUpVector() * -20;
	//FVector endLoc = startLoc + pos;

	//DrawDebugSphere(GetWorld(), endLoc,
	//fireDistance, 30, FColor::Cyan, false, -1, 0, 1);
}

void AVRPlayer::RotateRightAxis()
{
	UE_LOG(LogTemp, Error,TEXT("Rotator!!!!"));
	AddControllerYawInput(90);
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
	FVector handUp = FRotationMatrix(leftHand->GetComponentRotation()).GetUnitAxis(EAxis::X) * -1;

	FVector dir = handForward + handUp;

	FVector prediction = leftHand->GetComponentLocation() + dir * axis * speed * deltatime;
	//UE_LOG(LogTemp,Warning,TEXT("TimeSegment : %d & FireHand FVector : %s"), timeSegment,*prediction.ToString())

	leftHand->SetWorldLocation(prediction);

	FVector start = leftHand->GetComponentLocation();
	FVector end = leftHand->GetComponentLocation();

// 	DrawDebugSphere(GetWorld(), end,
// 	20, 30, FColor::Cyan, false, -1, 0, 1);

	FHitResult hitInfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, start, end, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeSphere(20), param);
	
	if (bHit)
	{
// 		AActor* actor = hitInfo.GetActor();
// 		ball = Cast<ABall>(actor);
//		UE_LOG(LogTemp, Warning, TEXT("hirInfo = %s"), *actor->GetName())
		
		UPrimitiveComponent* compHit = hitInfo.GetComponent();
		if (compHit->IsSimulatingPhysics() == true)
		{
			FVector dist = hitInfo.ImpactPoint - GetActorLocation();
			FVector hitDir = compHit->GetComponentLocation() - hitInfo.ImpactPoint;
			FVector Loc = dist + hitDir;
			FVector force = compHit->GetMass() * Loc * 300;
			compHit->AddForceAtLocation(force, hitInfo.ImpactPoint);
		}
		AActor* actor = hitInfo.GetActor(); 
		if (actor->GetName().Contains(TEXT("DM")))
		{
			bIsFire = false;
		}
	}

	//FVector direction = startPos - leftHand->GetComponentLocation();
	//if (direction.Length() > goalDir)
	//{
	//	bIsFire = false;
	//}
}

void AVRPlayer::ReturnRightHand()
{
	UE_LOG(LogTemp,Warning,TEXT("returnRigth!!!!!"))

	bIsFire = false;
}

void AVRPlayer::ReturnMove(float deltatime)
{
	//UE_LOG(LogTemp, Error, TEXT("deltaTime = %f"), deltatime)
	FVector returnPos = FMath::Lerp(currentPos, startPos, deltatime * 30);
	leftHand->SetWorldLocation(returnPos);
	//UE_LOG(LogTemp, Warning, TEXT("returnPos = %f,%f,%f"), returnPos.X, returnPos.Y, returnPos.Z);
}

void AVRPlayer::FindAngle()
{
	FVector dir = ballLoc - GetActorLocation();
	dir.Normalize();
 	float dotValue = FVector::DotProduct(GetActorForwardVector(), dir);
 	float angle = UKismetMathLibrary::DegAcos(dotValue);

	FVector cross = FVector::CrossProduct(GetActorForwardVector(), dir);
	float turnAngle = 0;

	if (cross.Z < 0)
	{
		turnAngle = angle * -1;
	}
	else
	{
		turnAngle = angle;
	}

	dirUI->ArrowRotation(turnAngle);
}
