// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include "RightHand.h"
#include <Components/ChildActorComponent.h>
#include "LeftHand.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	rightChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("RightHand"));
	rightChild->SetupAttachment(GetMesh());
	leftChild =CreateDefaultSubobject<UChildActorComponent>(TEXT("LeftHand"));
	leftChild->SetupAttachment(GetMesh());


	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkel(TEXT("/Script/Engine.SkeletalMesh'/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP'"));
	if (tempSkel.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempSkel.Object);
	}

	ConstructorHelpers::FClassFinder<ARightHand> tempRH(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_RightHand.BP_RightHand_C'"));
	if (tempRH.Succeeded())
	{
		rightHand = tempRH.Class;
	}

	ConstructorHelpers::FClassFinder<ALeftHand> tempLH(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_LeftHand.BP_LeftHand_C'"));
	if (tempLH.Succeeded())
	{
		leftHand = tempLH.Class;
	}

	rightChild->SetChildActorClass(rightHand);
	leftChild->SetChildActorClass(leftHand);
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



