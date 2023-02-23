// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalKeeper.h"
#include <Components/CapsuleComponent.h>
#include <Engine/SkeletalMesh.h>
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include "Ball.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Sound/SoundBase.h>
#include <Engine/StaticMesh.h>

// Sets default values
AGoalKeeper::AGoalKeeper()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyPreset"));
	GetCapsuleComponent()->SetCapsuleRadius(80);
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.5f));

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeScale3D(FVector(0.5f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	hand = CreateDefaultSubobject<UBoxComponent>("Hand");
	hand->SetupAttachment(GetMesh(), TEXT("HandPos"));
	hand->SetCollisionProfileName("HandPreset");
	hand->SetBoxExtent(FVector(50));
	hand->SetRelativeScale3D(FVector(1));


	handMesh = CreateDefaultSubobject<UStaticMeshComponent>("HandMesh");
	handMesh->SetupAttachment(hand);
	handMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	handMesh->SetVisibility(false);
	handMesh->SetRelativeScale3D(FVector(0.4f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/EnemyAssets/GoalKeeper/Hand/GoalKeeperHand.GoalKeeperHand'"));
	if (tempMesh.Succeeded())
	{
		handMesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/EnemySound/hitballSound.hitballSound'"));
	if (tempSound.Succeeded())
	{
		hitBallSound = tempSound.Object;
	}
}

// Called when the game starts or when spawned
void AGoalKeeper::BeginPlay()
{
	Super::BeginPlay();

	ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));

	hand->OnComponentBeginOverlap.AddDynamic(this, &AGoalKeeper::OnOverlapBegin);
}

// Called every frame
void AGoalKeeper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	dir = ball->GetActorLocation() - GetActorLocation();
	FRotator rot = UKismetMathLibrary::MakeRotFromX(dir);
	rot.Pitch = 0;
	rot.Roll = 0;
	FRotator rotHand = UKismetMathLibrary::MakeRotFromX(dir);
	SetActorRotation(rot);
	hand->SetWorldRotation(rotHand);

	if (dir.Length() < 1000)
	{
		BlockHand(DeltaTime * 15);
	}
	else if (dir.Length() > 1300)
	{
		ReturnHand();
	}
}

// Called to bind functionality to input
void AGoalKeeper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void AGoalKeeper::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherComp->GetName().Contains(TEXT("Ball")))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitBallSound, ball->GetActorLocation());
		FVector dirBlock = GetActorForwardVector() + GetActorUpVector();
		FVector F = ballMass * dirBlock * 1500;
		OtherComp->AddImpulse(F);
		ReturnHand();
	}
	else
	{
		ReturnHand();
	}
}

void AGoalKeeper::BlockHand(float speed)
{
	if (FMath::FRand() < 0.75f)
	{
		bBlock = true;
		handMesh->SetVisibility(true);
		clamp = FMath::Clamp(speed, 0.0f, 1.0f);
		FVector start = hand->GetComponentLocation();
		FVector end = ball->GetActorLocation();
		FVector lerp = FMath::Lerp(start, end, clamp);
		hand->SetWorldLocation(lerp);
	}
}

void AGoalKeeper::ReturnHand()
{
	//UE_LOG(LogTemp, Warning, TEXT("ReturnHand"));
	handMesh->SetVisibility(false);
	hand->SetWorldLocation(GetMesh()->GetSocketLocation(TEXT("HandPos")));
	bBlock = false;
}