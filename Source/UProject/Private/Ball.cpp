// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include "VRPlayer.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "CenterBallWidgetActor.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ball = CreateDefaultSubobject<USphereComponent>(TEXT("Ball"));
	SetRootComponent(ball);
	ball->SetSphereRadius(160);
	ball->SetRelativeScale3D(FVector(0.8f));
	ball->SetSimulatePhysics(true);
	ball->SetCollisionProfileName(TEXT("BallPreset"));
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(ball);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/FPWeapon/Mesh/FirstPersonProjectileMesh.FirstPersonProjectileMesh'"));
	if (tempMesh.Succeeded())
	{
		mesh->SetStaticMesh(tempMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"));
	if (tempMat.Succeeded())
	{
		mesh->SetMaterial(0, tempMat.Object);
	}
	ConstructorHelpers::FClassFinder<ACenterBallWidgetActor> tempWidget(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_CenterBallWidgetActor.BP_CenterBallWidgetActor_C'"));
	if (tempWidget.Succeeded())
	{
		widgetActor = tempWidget.Class;
	}
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
	player = Cast<AVRPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AVRPlayer::StaticClass()));
	enemy = Cast<AEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemy::StaticClass()));
	
	int32 rand = FMath::RandRange(-1600, 1600);
	ball->SetWorldLocation(FVector(0, rand, 800));
	ball->SetWorldRotation(FRotator(0));

	ball->OnComponentBeginOverlap.AddDynamic(this, &ABall::OnOverlapBegin);
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	startPos = FVector(-4500, -3000, -200);
	endPos = FVector(4500, 3000, 1600);

	float x = FMath::Clamp(GetActorLocation().X, startPos.X, endPos.X);
	float y = FMath::Clamp(GetActorLocation().Y, startPos.Y, endPos.Y);
	float z = FMath::Clamp(GetActorLocation().Z, startPos.Z, endPos.Z);
	FVector clampLoc = FVector(x, y, z);
	SetActorLocation(clampLoc);
}

void ABall::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetName().Contains(TEXT("goalPost")))
	{
		bGoal = true;
		mesh->SetVisibility(false);
		GetWorldTimerManager().SetTimer(goalHandle, this, &ABall::CenterBall, 3, false);
		GetWorld()->GetFirstPlayerController()->PlayHapticEffect(goalHaptic, EControllerHand::AnyHand, 1, false);
	}
}

void ABall::CenterBall()
{
	GetWorld()->SpawnActor<ACenterBallWidgetActor>(widgetActor, FVector(0, 0, 500), FRotator(0, 180, 0));

	FTimerHandle WaitHandle;
	float WaitTime = 3.2; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			enemy->ResetPos();
			player->ResetPos();
			int32 rand = FMath::RandRange(-1600, 1600);
			ball->SetWorldLocation(FVector(0, rand, 800));
			ball->SetWorldRotation(FRotator(0));
			mesh->SetVisibility(true);
			bGoal = false;
		}), WaitTime, false);
}

