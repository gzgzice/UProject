// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectionWidget.h"
#include <UMG/Public/Components/Image.h>


void UDirectionWidget::ArrowRotation(float angle)
{
	Arrow_Img->SetRenderTransformAngle(angle);
}
