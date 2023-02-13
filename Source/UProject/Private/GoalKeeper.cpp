// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalKeeper.h"
#include <Components/CapsuleComponent.h>
#include <Engine/SkeletalMesh.h>
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include "Ball.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AGoalKeeper::AGoalKeeper()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyPreset"));

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	hand = CreateDefaultSubobject<UBoxComponent>("Hand");
	hand->SetupAttachment(GetMesh(), TEXT("HandPos"));
	hand->SetCollisionProfileName("HandPreset");
	hand->SetBoxExtent(FVector(50));
	hand->SetRelativeScale3D(FVector(0.3));
	hand->SetRelativeRotation(FRotator(0, 90, -90));

	handMesh = CreateDefaultSubobject<UStaticMeshComponent>("HandMesh");
	handMesh->SetupAttachment(hand);
	handMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	handMesh->SetVisibility(false);
}

// Called when the game starts or when spawned
void AGoalKeeper::BeginPlay()
{
	Super::BeginPlay();
	
	ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));

	hand->OnComponentBeginOverlap.AddDynamic(this, &AGoalKeeper::OnOverlapBegin);

	UE_LOG(LogTemp, Error, TEXT("%d, %d, %d"),hand->GetComponentLocation().X, hand->GetComponentLocation().Y, hand->GetComponentLocation().Z);
}

// Called every frame
void AGoalKeeper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	dir = ball->GetActorLocation() - GetActorLocation();
	//dir.Normalize();
	FRotator rot = UKismetMathLibrary::MakeRotFromX(dir);
	rot.Pitch = 0;
	rot.Roll = 0;
	SetActorRotation(rot);
	hand->SetWorldRotation(rot);
	
	if (dir.Length() < 1000)
	{
		//GetRand();
// 		if (blockState == 0)
// 		{
// 			block = FMath::RandRange(1, 100);
// 			blockState++;
// 		}
			BlockHand(DeltaTime*10);
	}
	UE_LOG(LogTemp, Warning, TEXT("%d, %d, %d"), hand->GetComponentLocation().X, hand->GetComponentLocation().Y, hand->GetComponentLocation().Z);
}

// Called to bind functionality to input
void AGoalKeeper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void AGoalKeeper::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlappedComp = hand;
	if (OtherComp->GetName().Contains(TEXT("Ball")))
	{
		 FVector dirBlock = GetActorForwardVector()+GetActorUpVector();
		 FVector F = ballMass * dirBlock * 1000;
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
	handMesh->SetVisibility(true);
	FVector start = hand->GetComponentLocation();
	FVector end = ball->GetActorLocation();
	FVector lerp = FMath::Lerp(start,end, speed);
	if (FMath::FRand() < 0.9f)
	{
		hand->SetWorldLocation(lerp);
	}
}

void AGoalKeeper::ReturnHand()
{
	UE_LOG(LogTemp, Warning, TEXT("ReturnHand"));
	blockState = 0;
	handMesh->SetVisibility(false);
	hand->SetWorldLocation(GetMesh()->GetSocketLocation(TEXT("HandPos")));
}