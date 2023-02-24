// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include <Components/CapsuleComponent.h>
#include <Engine/SkeletalMesh.h>
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include "Ball.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Animation/AnimInstance.h>
#include <Particles/ParticleSystem.h>
#include <Sound/SoundBase.h>


// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyPreset"));
	GetCapsuleComponent()->SetCapsuleRadius(80);
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.5f));

	GetMesh()->SetRelativeLocation(FVector(20, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetRelativeScale3D(FVector(0.5f));

	hand = CreateDefaultSubobject<UBoxComponent>("Hand");
	hand->SetupAttachment(GetMesh(), TEXT("HandPos"));
	hand->SetCollisionProfileName("HandPreset");
	hand->SetBoxExtent(FVector(50));
	hand->SetRelativeScale3D(FVector(0.7));


	handMesh = CreateDefaultSubobject<UStaticMeshComponent>("HandMesh");
	handMesh->SetupAttachment(hand);
	handMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	handMesh->SetVisibility(false);
	handMesh->SetRelativeScale3D(FVector(4));
	handMesh->SetRelativeLocation(FVector(-520, 350, -410));
	handMesh->SetRelativeRotation(FRotator(0, -90, 0));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSkel(TEXT("/Script/Engine.SkeletalMesh'/Game/EnemyAssets/Enemy/Enemy.Enemy'"));
	if (tempSkel.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempSkel.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/EnemyAssets/Enemy/Hand/EnemyHand.EnemyHand'"));
	if (tempMesh.Succeeded())
	{
		handMesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> tempAttack(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_laser01.P_ky_laser01'"));
	if (tempAttack.Succeeded())
	{
		attackEffect = tempAttack.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/EnemySound/hitballSound.hitballSound'"));
	if (tempSound.Succeeded())
	{
		hitBallSound = tempSound.Object;
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/EnemyAssets/Enemy/ABP_Enemy.ABP_Enemy_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));

	hand->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::HandOverlapBegin);

	originPos = GetActorLocation();
	//UE_LOG(LogTemp, Error, TEXT("%d , %d, %d"), originPos.X, originPos.Y, originPos.Z);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*if(ball->)*/
	FVector dir = ball->GetActorLocation() - GetActorLocation();
	dir.Normalize();
	FRotator rot = UKismetMathLibrary::MakeRotFromX(dir);
	rot.Pitch = 0;
	rot.Roll = 0;
	FRotator rotHand = UKismetMathLibrary::MakeRotFromX(dir);
	SetActorRotation(rot);
	hand->SetWorldRotation(rotHand);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::HandOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains(TEXT("Ball")))
	{
		bHitBall = true;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitBallSound, ball->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),attackEffect,hand->GetComponentLocation(),hand->GetComponentRotation()+FRotator(90,0,0), FVector3d(0.4));
	}
	else
	{
		bHitOther = true;
	}
}

void AEnemy::ResetPos()
{
	SetActorLocation(originPos);
	//UE_LOG(LogTemp, Warning, TEXT("%d , %d, %d"), originPos.X, originPos.Y, originPos.Z);
}

