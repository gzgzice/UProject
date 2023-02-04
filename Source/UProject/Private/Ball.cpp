// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(sphere);
	sphere->SetSphereRadius(160);
	sphere->SetRelativeScale3D(FVector(0.8f));
	sphere->SetSimulatePhysics(true);
	
	ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
	ball->SetupAttachment(sphere);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/FPWeapon/Mesh/FirstPersonProjectileMesh.FirstPersonProjectileMesh'"));
	if (tempMesh.Succeeded())
	{
		ball->SetStaticMesh(tempMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"));
	if (tempMat.Succeeded())
	{
		ball->SetMaterial(0, tempMat.Object);
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

}

