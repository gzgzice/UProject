// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueGoalParticleActor.h"
#include <Particles/ParticleSystemComponent.h>

// Sets default values
ABlueGoalParticleActor::ABlueGoalParticleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compParticle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	compParticle->SetRelativeScale3D(FVector(1, 5, 5));

	InitialLifeSpan = 2;
}

// Called when the game starts or when spawned
void ABlueGoalParticleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlueGoalParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

