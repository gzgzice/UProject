// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetBall.h"
#include <Components/BoxComponent.h>
#include "Ball.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AResetBall::AResetBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(compBox);
	compBox->SetCollisionProfileName(TEXT("ResetBallPreset"));
}

// Called when the game starts or when spawned
void AResetBall::BeginPlay()
{
	Super::BeginPlay();
	
	ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));

	compBox->OnComponentBeginOverlap.AddDynamic(this, &AResetBall::OnOverlapBegin);
}

// Called every frame
void AResetBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AResetBall::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetName().Contains(TEXT("Ball")))
	{
		ball->CenterBall();
	}
}

