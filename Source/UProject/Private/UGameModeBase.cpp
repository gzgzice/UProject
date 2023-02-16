// Fill out your copyright notice in the Description page of Project Settings.


#include "UGameModeBase.h"
#include "ScoreWidget.h"

void AUGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void AUGameModeBase::AddRedScore(int32 newscore)
{
	currRedScore += newscore;
	
	scoreUI->UpdateRedScoreUI(currRedScore);
}

void AUGameModeBase::AddBlueScore(int32 newscore)
{
	currBlueScore += newscore;

	scoreUI->UpdateBlueScoreUI(currBlueScore);
}

//AGameModeBase* mode = GetWorld()->GetAuthGameMode();
//AUGameModeBase* currMode = Cast<AUGameModeBase>(mode);