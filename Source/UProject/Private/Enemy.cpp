// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include <Components/CapsuleComponent.h>
#include <Components/ChildActorComponent.h>
#include <Engine/SkeletalMesh.h>
#include "Hand.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyPreset"));

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	hand = CreateDefaultSubobject<UChildActorComponent>(TEXT("Hand"));
	hand->SetupAttachment(GetMesh(), TEXT("HandPos"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkel(TEXT("/Script/Engine.SkeletalMesh'/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP'"));
	if (tempSkel.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempSkel.Object);
	}

	ConstructorHelpers::FClassFinder<AHand> tempHand(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Hand.BP_Hand_C'"));
	if (tempHand.Succeeded())
	{
		child = tempHand.Class;
	}


	hand->SetChildActorClass(child);
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



