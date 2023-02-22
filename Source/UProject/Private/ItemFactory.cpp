// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFactory.h"
#include <Kismet/GameplayStatics.h>
#include "Item.h"

// Sets default values
AItemFactory::AItemFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemFactory::BeginPlay()
{
	Super::BeginPlay();
	
	item = Cast<AItem>(UGameplayStatics::GetActorOfClass(GetWorld(), AItem::StaticClass()));

	createTime = FMath::RandRange(5, 10);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle,this,&AItemFactory::SpawnBall, createTime);
}

// Called every frame
void AItemFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemFactory::SpawnBall()
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AItem>(ItemFactory, GetActorLocation(), GetActorRotation(), param);
}

