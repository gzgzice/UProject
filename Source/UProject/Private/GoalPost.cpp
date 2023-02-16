// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalPost.h"
#include <Components/BoxComponent.h>
#include "Ball.h"
#include <Kismet/GameplayStatics.h>
#include "UGameModeBase.h"

// Sets default values
AGoalPost::AGoalPost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	goalPost = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalPost"));
	SetRootComponent(goalPost);
	goalPost->SetBoxExtent(FVector(50));
	goalPost->SetRelativeScale3D(FVector(8, 19.5f, 5));
	goalPost->SetCollisionProfileName(TEXT("GoalPostPreset"));
}

// Called when the game starts or when spawned
void AGoalPost::BeginPlay()
{
	Super::BeginPlay();
	
	ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
	gameMode = Cast<AUGameModeBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AUGameModeBase::StaticClass()));
}

// Called every frame
void AGoalPost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

