// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEffectActor.h"
#include <Particles/ParticleSystem.h>

// Sets default values
APlayerEffectActor::APlayerEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

