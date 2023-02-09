// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include <Components/CapsuleComponent.h>
#include <Components/ChildActorComponent.h>
#include <Engine/SkeletalMesh.h>
#include "Hand.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include "Ball.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>


// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyPreset"));

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	hand = CreateDefaultSubobject<UBoxComponent>("Hand");
	hand->SetupAttachment(GetMesh(), TEXT("HandPos"));
	hand->SetCollisionProfileName("HandPreset");
	hand->SetBoxExtent(FVector(50));
	hand->SetRelativeScale3D(FVector(0.3));
	

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>("HandMesh");
	compMesh->SetupAttachment(hand);
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	compMesh->SetVisibility(false);
	//ConstructorHelpers::FObjectFinder

// 	hand = CreateDefaultSubobject<UChildActorComponent>(TEXT("Hand"));
// 	hand->SetupAttachment(GetMesh(), TEXT("HandPos"));
// 	hand->SetRelativeRotation(FRotator(0,90,-90));
// 
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkel(TEXT("/Script/Engine.SkeletalMesh'/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP'"));
	if (tempSkel.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempSkel.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}
// 	ConstructorHelpers::FClassFinder<AHand> tempHand(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Hand.BP_Hand_C'"));
// 	if (tempHand.Succeeded())
// 	{
// 		child = tempHand.Class;
// 	}
// 
// 
// 	hand->SetChildActorClass(child);
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector dir = ball->GetActorLocation() - GetActorLocation();
	dir.Normalize();
	FRotator rot = UKismetMathLibrary::MakeRotFromX(dir);
	rot.Pitch = 0;
	rot.Roll = 0;
	SetActorRotation(rot);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


