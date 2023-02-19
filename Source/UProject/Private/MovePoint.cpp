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
	compBox->SetRelativeScale3D(FVector(2.5f, 2.5f, 0.01f));
	compBox->SetCollisionProfileName(TEXT("BlockAll"));

	caughtBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CaughtCollision"));
	caughtBox->SetupAttachment(compBox);
	caughtBox->SetCollisionProfileName(TEXT("PointPreset"));
	caughtBox->SetCapsuleHalfHeight(70000);
	caughtBox->SetCapsuleRadius(20);
	caughtBox->SetRelativeScale3D(FVector(0, 0, 68500));

	compMesh0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	compMesh0->SetupAttachment(compBox);
	compMesh0->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	compMesh0->SetRelativeScale3D(FVector(0.8f));

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	compMesh->SetupAttachment(compBox);
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	compMesh->SetRelativeLocation(FVector(30, 0, 0));
	compMesh->SetRelativeScale3D(FVector(0.05f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh0(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (tempMesh0.Succeeded())
	{
		compMesh0->SetStaticMesh(tempMesh0.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempFloor(TEXT("/Script/Engine.Material'/Interchange/Materials/UnlitMaterial.UnlitMaterial'"));
	if (tempFloor.Succeeded())
	{
		compMesh0->SetMaterial(0, tempFloor.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/EnemyAssets/Point/MovePoint.MovePoint'"));
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

