// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ball = CreateDefaultSubobject<USphereComponent>(TEXT("Ball"));
	SetRootComponent(ball);
	ball->SetSphereRadius(160);
	ball->SetRelativeScale3D(FVector(0.8f));
	ball->SetSimulatePhysics(true);
	ball->SetCollisionProfileName(TEXT("BallPreset"));
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(ball);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/FPWeapon/Mesh/FirstPersonProjectileMesh.FirstPersonProjectileMesh'"));
	if (tempMesh.Succeeded())
	{
		mesh->SetStaticMesh(tempMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"));
	if (tempMat.Succeeded())
	{
		mesh->SetMaterial(0, tempMat.Object);
	}
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float x = FMath::Clamp(GetActorLocation().X, startPos.X, endPos.X);
	float y = FMath::Clamp(GetActorLocation().Y, startPos.Y, endPos.Y);
	float z = FMath::Clamp(GetActorLocation().Z, startPos.Z, endPos.Z);
	FVector clampLoc = FVector(x, y, z);
	SetActorLocation(clampLoc);
}

