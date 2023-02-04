// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "VRPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "Ball.h"
#include "Enemy.h"

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

	if (bComplete)
	{
		//�˻� ���·� ��ȯ�Ѵ�.
		ChangeState(EEnemyState::Search);
	}
}

void UEnemyFSM::SearchState()
{
	FVector dir = ball->GetActorLocation() - enemy->GetActorLocation();
	//1.���� searchRange �ȿ� Ball �� �ִٸ�
	if (dir.Length() < searchRange)
	{
		//2.���� ���·� ��ȯ�Ѵ�.
		ChangeState(EEnemyState::Attack);
	}
	//3.searchRange �ȿ� Ball �� ���ٸ�
	else
	{
		//4.�̵� ���·� ��ȯ�Ѵ�.
		ChangeState(EEnemyState::Move);
	}
}

void UEnemyFSM::MoveState()
{

}

void UEnemyFSM::AttackState()
{

}

void UEnemyFSM::ChangeState(EEnemyState afterState)
{
	state = afterState;

	switch (state)
	{
		case EEnemyState::Search:
		{

		}
		break;
		case EEnemyState::Move:
		{

		}
		break;
		case EEnemyState::Attack:
		{

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
