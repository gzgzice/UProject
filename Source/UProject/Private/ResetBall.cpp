// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetBall.h"
#include <Components/BoxComponent.h>
#include "Ball.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SphereComponent.h>

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
// 		UE_LOG(LogTemp, Warning, TEXT("RESET!!!!!"));
// 		ball->mesh->SetVisibility(false);
// 		ball->ball->SetCollisionProfileName(TEXT("BallNoColl"));

		int32 rand1 = FMath::RandRange(-1000,1000);
		int32 rand2 = FMath::RandRange(200,800);
		ball->SetActorLocation(FVector(rand1, rand1, rand2));
		ball->SetActorRotation(FRotator(0));
	}
}

