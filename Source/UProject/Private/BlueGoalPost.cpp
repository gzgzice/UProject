// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueGoalPost.h"
#include <Components/BoxComponent.h>
#include "UGameModeBase.h"
#include "Ball.h"
#include <Components/SphereComponent.h>

ABlueGoalPost::ABlueGoalPost()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABlueGoalPost::BeginPlay()
{
	goalPost->OnComponentBeginOverlap.AddDynamic(this, &ABlueGoalPost::BallOverlapBegin);
}

void ABlueGoalPost::BallOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetName().Contains(TEXT("Ball")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Red Goal!!"));
		//gameMode->AddRedScore(1);

	}
}
