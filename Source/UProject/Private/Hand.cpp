// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include "Enemy.h"

// Sets default values
AHand::AHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hand = CreateDefaultSubobject<UBoxComponent>(TEXT("Hand"));
	SetRootComponent(hand);
	hand->SetBoxExtent(FVector(50));
	hand->SetRelativeScale3D(FVector(0.3f));
	hand->SetCollisionProfileName(TEXT("HandPreset"));

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	compMesh->SetupAttachment(hand);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void AHand::BeginPlay()
{
	Super::BeginPlay();
	
	SetActive(false);


}

// Called every frame
void AHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AHand::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

}

void AHand::SetActive(bool isActive)
{
	if (isActive == true)
	{
		//생성된 총알을 보이게 하자
		compMesh->SetVisibility(true);
		//생성된 총알의 충돌을 가능하게 하자
		hand->SetCollisionProfileName(TEXT("HandPreset"));
	}
	else
	{
		//생성된 총알을 보이지 않게 한다.
		compMesh->SetVisibility(false);
		//생성된 총알의 충돌 옵션을 NoCollision 으로 한다.
		hand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}