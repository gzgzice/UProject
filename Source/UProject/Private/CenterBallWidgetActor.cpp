// Fill out your copyright notice in the Description page of Project Settings.


#include "CenterBallWidgetActor.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include <UMG/Public/Blueprint/UserWidget.h>

// Sets default values
ACenterBallWidgetActor::ACenterBallWidgetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//À§Á¬ÄÄÆ÷³ÍÆ® »ý¼º
	centerWG = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

	ConstructorHelpers::FClassFinder<UUserWidget>UserWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/BP_CenterBallWidget.BP_CenterBallWidget_C'"));
	if (UserWidget.Succeeded())
	{
		centerWG->SetWidgetClass(UserWidget.Class);
		centerWG->SetDrawSize(FVector2D(2000.0f, 2000.0f));
	}

	InitialLifeSpan = 3.5f;
}

// Called when the game starts or when spawned
void ACenterBallWidgetActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACenterBallWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

