// Fill out your copyright notice in the Description page of Project Settings.


#include "RedGoalPost.h"
#include <Components/BoxComponent.h>
#include "UGameModeBase.h"
#include "Ball.h"
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>

ARedGoalPost::ARedGoalPost()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARedGoalPost::BeginPlay()
{
	goalPost->OnComponentBeginOverlap.AddDynamic(this, &ARedGoalPost::BallOverlapBegin);
}

void ARedGoalPost::BallOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetName().Contains(TEXT("Ball")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Blue Goal!!"));
		//gameMode->AddBlueScore(1);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), goalEffect, GetActorTransform());
	}
}
