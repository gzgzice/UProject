// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "VRPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "Ball.h"
#include "Enemy.h"
#include <Components/BoxComponent.h>
#include "Engine/World.h"
#include "MovePoint.h"
#include "BlueGoalPost.h"
#include <Particles/ParticleSystem.h>
#include <Sound/SoundBase.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UParticleSystem> tempMove(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_shotShockwave.P_ky_shotShockwave'"));
	if (tempMove.Succeeded())
	{
		moveEffect = tempMove.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound1(TEXT("/Script/Engine.SoundWave'/Game/EnemySound/attacksound.attacksound'"));
	if (tempSound1.Succeeded())
	{
		attackSound = tempSound1.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound2(TEXT("/Script/Engine.SoundWave'/Game/EnemySound/movesound.movesound'"));
	if (tempSound2.Succeeded())
	{
		moveSound = tempSound2.Object;
	}
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AVRPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AVRPlayer::StaticClass()));
	enemy = Cast<AEnemy>(GetOwner());
	ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
	point = Cast<AMovePoint>(UGameplayStatics::GetActorOfClass(GetWorld(), AMovePoint::StaticClass()));
	blueGoalPost = Cast<ABlueGoalPost>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlueGoalPost::StaticClass()));
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (state)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Search:
		SearchState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	}
}

void UEnemyFSM::IdleState()
{
	
	if (FlowTime(idleDelayTIme))
	{
		//검색 상태로 전환한다.
		ChangeState(EEnemyState::Search);
	}
}

void UEnemyFSM::SearchState()
{
	if (FlowTime(searchDelayTIme))
	{
		FVector dir = ball->GetActorLocation() - enemy->GetActorLocation();
		//1.만약 searchRange 안에 Ball 이 있다면
		if (dir.Length() < searchRange)
		{
			//2.공격 상태로 전환한다.
			ChangeState(EEnemyState::Attack);
		}
		//3.searchRange 안에 Ball 이 없다면
		else
		{
			//4.이동 상태로 전환한다.
			ChangeState(EEnemyState::Move);
		}
	}	
}

void UEnemyFSM::MoveState()
{

	FVector startLoc = enemy->GetActorLocation() + enemy->GetActorForwardVector() * 300;
	FVector endLoc = enemy->GetActorLocation() + enemy->GetActorForwardVector() * 1500;
	FHitResult hitPoint;
	FCollisionObjectQueryParams objectPoint;
	objectPoint.AddObjectTypesToQuery(ECC_GameTraceChannel6);
	FHitResult hitEnemy;
	FCollisionObjectQueryParams objectEnemy;
	objectEnemy.AddObjectTypesToQuery(ECC_GameTraceChannel4);
	FHitResult hitPlayer;
	FCollisionObjectQueryParams objectPlayer;
	objectPlayer.AddObjectTypesToQuery(ECC_Pawn);


	//DrawDebugSphere(GetWorld(), endLoc, 75, 30, FColor::Cyan, false, 5, 0, 1);

	bool bPoint = GetWorld()->SweepSingleByObjectType(hitPoint, startLoc, endLoc, FQuat::Identity, objectPoint, FCollisionShape::MakeSphere(80));
	bool bEnemy = GetWorld()->SweepSingleByObjectType(hitEnemy, startLoc, endLoc, FQuat::Identity, objectEnemy, FCollisionShape::MakeSphere(80));
	bool bPlayer = GetWorld()->SweepSingleByObjectType(hitPlayer, startLoc, endLoc, FQuat::Identity, objectPlayer, FCollisionShape::MakeSphere(80));

	if (bEnemy || bPlayer)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Enemy or Player or Ball"));
		ChangeState(EEnemyState::Idle);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("None_Enemy"));
		if (bPoint)
		{
			AActor* actorPoint = hitPoint.GetActor();
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *actorPoint->GetName());
			FVector arriveLoc = actorPoint->GetActorLocation();
			arriveLoc.Z = 135;
				if (actorPoint->GetName().Contains(TEXT("Point")))
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), moveSound, enemy->GetActorLocation());
					enemy->SetActorLocation(arriveLoc);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), moveEffect,enemy->GetActorLocation());
					ChangeState(EEnemyState::Idle);
				}
		}
		else
		{
			ChangeState(EEnemyState::Idle);
		}
	}
}

void UEnemyFSM::AttackState()
{
	FVector start = enemy->GetActorLocation() + FVector(0, 0, 100);
	FVector end = ball->GetActorLocation() + FVector(x, y, z);
	FHitResult hit;
	FCollisionObjectQueryParams objectBall;
	objectBall.AddObjectTypesToQuery(ECC_GameTraceChannel2);

	bool bHitball = GetWorld()->LineTraceSingleByObjectType(hit, start, end, objectBall);
	
//DrawDebugLine(GetWorld(),start, end, FColor::Blue, false, 3,0,1);
	if (bHitball)
	{
		UPrimitiveComponent* hitBall = hit.GetComponent();
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *hit.GetComponent()->GetName());
		attackSpeed += GetWorld()->GetDeltaSeconds();
		float a = FMath::Clamp(attackSpeed, 0.0f, 1.0f);
		FVector dirForce = ball->GetActorLocation() - enemy->hand->GetComponentLocation();
		FVector attackStart = enemy->hand->GetComponentLocation();
		FVector p = FMath::Lerp(attackStart, end, a/2);
		enemy->hand->SetWorldLocation(p);

		if (enemy->bHitBall)
		{
			FVector	ballLocation = hit.Component->GetComponentLocation();
			FVector postLocation = blueGoalPost->GetActorLocation();
			FVector dirGoalPost = (postLocation + FVector(x, y, z)) -ballLocation;
			if (dirGoalPost.Length() > 2000)
			{
				if (hit.Component->IsSimulatingPhysics())
				{
					//UE_LOG(LogTemp, Error, TEXT("%s"), *hit.GetComponent()->GetName());
					FVector F = hit.GetComponent()->GetMass() * dirForce * 1300;
					hit.GetComponent()->AddForceAtLocation(F, hit.ImpactPoint);
					ReturnHand();
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("None Simulate"));
					ReturnHand();
				}
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Go GoalPost"));
				//DrawDebugLine(GetWorld(), ballLocation, postLocation, FColor::Green, false, 3, 0, 1);
				if (hit.Component->IsSimulatingPhysics())
				{
					//UE_LOG(LogTemp, Error, TEXT("%s"), *hit.GetComponent()->GetName());
					FVector F = hit.GetComponent()->GetMass() * dirGoalPost * 5;
					hit.GetComponent()->AddImpulse(F);
					ReturnHand();
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("None Simulate"));
					ReturnHand();
				}
			}		
		}
	}
	else if(enemy->bHitOther)
	{
		if (FlowTime(3))
		{
			ChangeState(EEnemyState::Idle);
		}
	}
}

void UEnemyFSM::ChangeState(EEnemyState afterState)
{
	state = afterState;

	switch (state)
	{
	case EEnemyState::Idle:
	{
		enemy->bHitBall = false;
		enemy->bHitOther = false;
		enemy->handMesh->SetVisibility(false);
		//UE_LOG(LogTemp, Warning, TEXT("IDLE"));
	}
	case EEnemyState::Search:
	{
		//UE_LOG(LogTemp, Warning, TEXT("SEARCH"));
	}
	break;
	case EEnemyState::Move:
	{
		//UE_LOG(LogTemp, Warning, TEXT("MOVE"));
	}
	break;
	case EEnemyState::Attack:
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), attackSound, enemy->GetActorLocation());
		x = FMath::RandRange(-90, 90);
		y = FMath::RandRange(-90, 90);
		z = FMath::RandRange(-90, 90);
		enemy->handMesh->SetVisibility(true);
		//UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
	}
	break;
	}
}

bool UEnemyFSM::FlowTime(float delayTime)
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > delayTime)
	{
		currentTime = 0;
		return true;
	}
	return false;
}

void UEnemyFSM::ReturnHand()
{
	//UE_LOG(LogTemp, Warning, TEXT("ReturnHand"));
	enemy->hand->SetWorldLocation(enemy->GetMesh()->GetSocketLocation(TEXT("HandPos")));
	ChangeState(EEnemyState::Idle);
}


