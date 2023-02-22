// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"
#include <UMG/Public/Components/Button.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	Btn_Start->OnPressed.AddDynamic(this, &UStartWidget::OnPress);
	Btn_Exit->OnPressed.AddDynamic(this, &UStartWidget::OnRelease);
}

void UStartWidget::OnPress()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMap"));
}

void UStartWidget::OnRelease()
{
	UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
}
