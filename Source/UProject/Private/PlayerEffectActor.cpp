// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEffectActor.h"
#include <Particles/ParticleSystemComponent.h>
#include <Particles/ParticleSystem.h>

// Sets default values
APlayerEffectActor::APlayerEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	playerEffect = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	playerEffect->SetRelativeScale3D(FVector(1, 5, 5));
	// 	ConstructorHelpers::FObjectFinder<UParticleSystem> tempParticle(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_fireStorm.P_ky_fireStorm'"));
	// 	if (tempParticle.Succeeded)
	// 	{
	// 		compParticle->SetTemplate(tempParticle.Object);
	// 	}

	InitialLifeSpan = 5;
}

// Called when the game starts or when spawned
void APlayerEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

