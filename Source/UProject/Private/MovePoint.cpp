// Fill out your copyright notice in the Description page of Project Settings.


#include "MovePoint.h"
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>

// Sets default values
AMovePoint::AMovePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(compBox);
	compBox->SetCollisionProfileName(TEXT("BlockAll"));

	caughtBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CaughtCollision"));
	caughtBox->SetupAttachment(compBox);
	caughtBox->SetCollisionProfileName(TEXT("PointPreset"));
	caughtBox->SetRelativeLocation(FVector(0,0,16000));
	caughtBox->SetRelativeScale3D(FVector(0.5f,0.5f,500));

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	compMesh->SetupAttachment(compBox);
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

