// Fill out your copyright notice in the Description page of Project Settings.


#include "MovePoint.h"
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include <Components/CapsuleComponent.h>
#include <Materials/Material.h>

// Sets default values
AMovePoint::AMovePoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(compBox);
	//compBox->SetRelativeScale3D(FVector(2.5f, 2.5f, 0.01f));
	compBox->SetRelativeScale3D(FVector(3.5f, 3.5f, 0.01f));
	compBox->SetCollisionProfileName(TEXT("BlockAll"));

	caughtBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CaughtCollision"));
	caughtBox->SetupAttachment(compBox);
	caughtBox->SetCollisionProfileName(TEXT("PointPreset"));
	caughtBox->SetCapsuleHalfHeight(10000);
	caughtBox->SetCapsuleRadius(20);
	//caughtBox->SetRelativeLocation(FVector(0, 0, 9000));
	caughtBox->SetRelativeScale3D(FVector(1.5f, 1.5f, 10));
	caughtBox->SetRelativeLocation(FVector(0, 0, 97500));


	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	compMesh->SetupAttachment(compBox);
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	compMesh->SetRelativeScale3D(FVector(0.5f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Blueprints/Redpoint.Redpoint'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void AMovePoint::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMovePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

