// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueGoalPost.h"
#include <Components/BoxComponent.h>		
#include "Ball.h"		
#include <Kismet/GameplayStatics.h>		
#include "ScoreWidget.h"		
#include "ScoreWidgetActor.h"		
#include <UMG/Public/Components/WidgetComponent.h>
#include "RedGoalParticleActor.h"

// Sets default values
ABlueGoalPost::ABlueGoalPost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	goalPost = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalPost"));
	SetRootComponent(goalPost);
	goalPost->SetBoxExtent(FVector(50));
	goalPost->SetRelativeScale3D(FVector(8, 19.5f, 5));
	goalPost->SetCollisionProfileName(TEXT("GoalPostPreset"));

	ConstructorHelpers::FClassFinder<ARedGoalParticleActor> tempGoal(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_RedGoalParticleActor.BP_RedGoalParticleActor_C'"));
	if (tempGoal.Succeeded())
	{
		goalEffect = tempGoal.Class;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/EnemySound/goalsound1.goalsound1'"));
	if (tempSound.Succeeded())
	{
		goalSound = tempSound.Object;
	}
}

// Called when the game starts or when spawned
void ABlueGoalPost::BeginPlay()
{
	Super::BeginPlay();
	
	ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
	ScoreWidgetActor = Cast<AScoreWidgetActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AScoreWidgetActor::StaticClass()));
	scoreWidget = Cast<UScoreWidget>(ScoreWidgetActor->scoreWG->GetUserWidgetObject());

	goalPost->OnComponentBeginOverlap.AddDynamic(this, &ABlueGoalPost::BallOverlapBegin);
}

// Called every frame
void ABlueGoalPost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlueGoalPost::BallOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetName().Contains(TEXT("Ball")))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Red Goal!!"));
		UGameplayStatics::PlaySound2D(GetWorld(), goalSound);

		GetWorld()->SpawnActor<ARedGoalParticleActor>(goalEffect, GetActorLocation(), GetActorRotation()+FRotator(-90,0,0));
		
		for (int32 i = 0; i < WidgetActorArray.Num(); i++)
		{
			WidgetActorArray[i]->ReceiveRedScore(1);
		}
	}
}


