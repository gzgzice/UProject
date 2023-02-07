// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "VRPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "Ball.h"
#include "Enemy.h"
#include "Hand.h"

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
	enemy->hand->SetActive(false);

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
 //point
}

void UEnemyFSM::AttackState()
{
	attackSpeed += GetWorld()->GetDeltaSeconds();
	FVector P = FMath::Lerp(enemy->hand->GetComponentLocation(), ball->GetActorLocation(), attackSpeed);
	enemy->hand->SetWorldLocation(P);
}

void UEnemyFSM::ChangeState(EEnemyState afterState)
{
	state = afterState;

	switch (state)
	{
		case EEnemyState::Idle:
		{
			UE_LOG(LogTemp, Warning, TEXT("IDLE"));
		}
		case EEnemyState::Search:
		{
			UE_LOG(LogTemp, Warning, TEXT("SEARCH"));
		}
		break;
		case EEnemyState::Move:
		{
			enemy->hand->SetActive(true);
			UE_LOG(LogTemp, Warning, TEXT("MOVE"));
		}
		break;
		case EEnemyState::Attack:
		{
			
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
