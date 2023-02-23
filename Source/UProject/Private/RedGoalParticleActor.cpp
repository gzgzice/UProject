// Fill out your copyright notice in the Description page of Project Settings.


#include "RedGoalParticleActor.h"
#include <Particles/ParticleSystemComponent.h>
#include <Particles/ParticleSystem.h>

// Sets default values
ARedGoalParticleActor::ARedGoalParticleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compParticle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	compParticle->SetRelativeScale3D(FVector(1,5,5));
// 	ConstructorHelpers::FObjectFinder<UParticleSystem> tempParticle(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_fireStorm.P_ky_fireStorm'"));
// 	if (tempParticle.Succeeded)
// 	{
// 		compParticle->SetTemplate(tempParticle.Object);
// 	}

	InitialLifeSpan = 2;
}

// Called when the game starts or when spawned
void ARedGoalParticleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARedGoalParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

