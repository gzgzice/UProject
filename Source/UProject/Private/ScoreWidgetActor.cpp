// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidgetActor.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include "ScoreWidget.h"

// Sets default values
AScoreWidgetActor::AScoreWidgetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//À§Á¬ÄÄÆ÷³ÍÆ® »ý¼º
	scoreWG = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

	ConstructorHelpers::FClassFinder<UUserWidget>UserWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/BP_ScoreUI.BP_ScoreUI_C'"));
	if (UserWidget.Succeeded())
	{
		scoreWG->SetWidgetClass(UserWidget.Class);
		scoreWG->SetDrawSize(FVector2D(2000.0f, 1000.0f));
	}
}

// Called when the game starts or when spawned
void AScoreWidgetActor::BeginPlay()
{
	Super::BeginPlay();

	scoreUI = Cast<UScoreWidget>(scoreWG->GetUserWidgetObject());
	
}

// Called every frame
void AScoreWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScoreWidgetActor::ReceiveBlueScore(int32 score)
{
	scoreUI->UpdateBlueScoreUI(score);
}

void AScoreWidgetActor::ReceiveRedScore(int32 score)
{
	scoreUI->UpdateRedScoreUI(score);
}

