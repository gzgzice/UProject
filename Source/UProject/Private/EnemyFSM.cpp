// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "VRPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "Ball.h"
#include "Enemy.h"
#include "Hand.h"
#include <Components/BoxComponent.h>
#include "Engine/World.h"
#include "MovePoint.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AVRPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AVRPlayer::StaticClass()));
	enemy = Cast<AEnemy>(GetOwner());
	ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
	hand = Cast<AHand>(UGameplayStatics::GetActorOfClass(GetWorld(), AHand::StaticClass()));
	point = Cast<AMovePoint>(UGameplayStatics::GetActorOfClass(GetWorld(), AMovePoint::StaticClass()));
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
	bool bComplete = FlowTime(idleDelayTIme);

	DrawDebugLine(GetWorld(), enemy->GetActorLocation() + enemy->GetActorForwardVector() * 150,
		enemy->GetActorLocation() + enemy->GetActorForwardVector() * 750, FColor::Red, true, -1, 0, 2);

	if (bComplete)
	{
		//검색 상태로 전환한다.
		ChangeState(EEnemyState::Search);
	}
}

void UEnemyFSM::SearchState()
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

void UEnemyFSM::MoveState()
{

	FVector startLoc = enemy->GetActorLocation() + enemy->GetActorForwardVector() * 220;
	//FVector pos1 = enemy->GetActorUpVector() * -50;
	FVector endLoc = enemy->GetActorLocation() + enemy->GetActorForwardVector() * 700;
	FHitResult hitPoint;
	FHitResult hitEnemy;

	DrawDebugSphere(GetWorld(), endLoc,
		75, 30, FColor::Cyan, true, -1, 0, 1);

	bool bPoint = GetWorld()->SweepSingleByChannel(hitPoint, startLoc, endLoc, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(75));
	bool bEnemy = GetWorld()->SweepSingleByProfile(hitEnemy, startLoc, endLoc, FQuat::Identity, TEXT("EnemyPreset"), FCollisionShape::MakeSphere(75));
//  	bool bPoint = GetWorld()->LineTraceSingleByChannel(hitPoint, startLoc, endLoc, ECC_Visibility);
//  	bool bEnemy = GetWorld()->LineTraceSingleByProfile(hitEnemy, startLoc, endLoc, TEXT("EnemyPreset"));

	if (bEnemy)
	{
			AActor* actorEnemy = hitEnemy.GetActor();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *actorEnemy->GetName());
			ChangeState(EEnemyState::Idle);
		
	}
	else
		{
			UE_LOG(LogTemp, Warning, TEXT("None_Enemy"));
			if (bPoint)
			{
				AActor* actorPoint = hitPoint.GetActor();
				FVector arriveLoc = actorPoint->GetActorLocation();
				arriveLoc.Z = 91;
				{
					if (actorPoint->GetName().Contains(TEXT("Point")))
					{
						UE_LOG(LogTemp, Warning, TEXT("%s"), *actorPoint->GetName());
						moveSpeed += GetWorld()->DeltaTimeSeconds;
						enemy->SetActorLocation(arriveLoc);
						ChangeState(EEnemyState::Idle);
					}
				}
			}
		}

}

void UEnemyFSM::AttackState()
{
	FVector start = enemy->GetActorLocation() + enemy->GetActorForwardVector();
	attackSpeed += GetWorld()->GetDeltaSeconds();
	FVector P = FMath::Lerp(start, ball->GetActorLocation(), attackSpeed);
	enemy->hand->SetWorldLocation(P);




	// 
	// 	FHitResult hitInfo;
	// 	FCollisionQueryParams param;
	// 	param.AddIgnoredActor(enemy);
	// 	//param.AddIgnoredActor(hand);
	// 
	// 	if (GetWorld()->LineTraceSingleByChannel(hitInfo, start, start + FVector(1, 0, 0), ECC_Visibility, param))
	// 	{
	// 
	// 				enemy->hand->SetWorldLocation(enemy->GetMesh()->GetSocketLocation(TEXT("HandPos")));
	// 		//GetWorldTimerManager().SetTimer(returnHandle, this, &UEnemyFSM::ReturnIdle, 0.5f, false);
	// 		float WaitTime = 0.03; //시간을 설정하고
	// 		GetWorld()->GetTimerManager().SetTimer(returnHandle, FTimerDelegate::CreateLambda([&]()
	// 			{
	// 				ChangeState(EEnemyState::Idle);
	// 			}), WaitTime, false);
	// 
	// 	}
}

void UEnemyFSM::ChangeState(EEnemyState afterState)
{
	state = afterState;

	switch (state)
	{
	case EEnemyState::Idle:
	{
		idleDelayTIme = 0;
		//enemy->compMesh->SetVisibility(false);
		UE_LOG(LogTemp, Warning, TEXT("IDLE"));
	}
	case EEnemyState::Search:
	{
		UE_LOG(LogTemp, Warning, TEXT("SEARCH"));
	}
	break;
	case EEnemyState::Move:
	{
		UE_LOG(LogTemp, Warning, TEXT("MOVE"));
	}
	break;
	case EEnemyState::Attack:
	{
		enemy->compMesh->SetVisibility(true);
		UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
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

void UEnemyFSM::ReturnIdle()
{
	ChangeState(EEnemyState::Idle);
}
