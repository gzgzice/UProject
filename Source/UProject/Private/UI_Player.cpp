// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player.h"
#include "HeadMountedDisplayFunctionLibrary.h"

// Sets default values
AUI_Player::AUI_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUI_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUI_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUI_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

