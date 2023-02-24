// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include "VRPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <UMG/Public/Components/WidgetInteractionComponent.h>

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	sphereComp->SetupAttachment(RootComponent);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(sphereComp);
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AVRPlayer* player = Cast<AVRPlayer>(OtherActor);
	if (player)
	{
		player->DetectBall(true);
		player->widgetPointer_right->bShowDebug = true;
		UGameplayStatics::PlaySound2D(GetWorld(), itemSound);
		Destroy();
	}
}



