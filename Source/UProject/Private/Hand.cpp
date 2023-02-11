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
		//������ �Ѿ��� ���̰� ����
		compMesh->SetVisibility(true);
		//������ �Ѿ��� �浹�� �����ϰ� ����
		hand->SetCollisionProfileName(TEXT("HandPreset"));
	}
	else
	{
		//������ �Ѿ��� ������ �ʰ� �Ѵ�.
		compMesh->SetVisibility(false);
		//������ �Ѿ��� �浹 �ɼ��� NoCollision ���� �Ѵ�.
		hand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}