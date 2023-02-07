// Fill out your copyright notice in the D,,scription page of Project Settings.


#include "RightHand.h"
#include <Components/BoxComponent.h>
#include <Engine/StaticMesh.h>

// Sets default values
ARightHand::ARightHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHand"));
	SetRootComponent(compBox);
	compBox->SetRelativeLocation(FVector(-90,0,140));
	compBox->SetRelativeRotation(FRotator(0,90,0));
	compBox->SetRelativeScale3D(FVector(0.3f));
	compBox->SetBoxExtent(FVector(50));
	compBox->SetCollisionProfileName(TEXT("HandPreset"));

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	compMesh-> SetupAttachment(compBox);
	compMesh->SetRelativeLocation(FVector(0, 0, -50));
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	ConstructorHelpers::FObjectFinder<UStaticMesh> tempChild(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (tempChild.Succeeded())
	{
		compMesh->SetStaticMesh(tempChild.Object);
	}
}

// Called when the game starts or when spawned
void ARightHand::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARightHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

