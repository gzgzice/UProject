// Fill out your copyright notice in the Description page of Project Settings.


#include "UGameModeBase.h"
#include "ScoreWidget.h"
#include "ScoreWidgetActor.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/WidgetComponent.h>

AUGameModeBase::AUGameModeBase()
{
	ScoreWidgetActor = Cast<AScoreWidgetActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AScoreWidgetActor::StaticClass()));

}

void AUGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//ScoreWidgetActor->scoreWG->InitWidget();
	//scoreUI = Cast<UScoreWidget>(ScoreWidgetActor->scoreWG->GetUserWidgetObject());
}

void AUGameModeBase::AddRedScore(int32 newscore)
{
	currRedScore += newscore;
	
	ScoreWidgetActor->scoreUI->UpdateRedScoreUI(currRedScore);
}

void AUGameModeBase::AddBlueScore(int32 newscore)
{
	currBlueScore += newscore;

	ScoreWidgetActor->scoreUI->UpdateBlueScoreUI(currBlueScore);
}

//AGameModeBase* mode = GetWorld()->GetAuthGameMode();
//AUGameModeBase* currMode = Cast<AUGameModeBase>(mode);