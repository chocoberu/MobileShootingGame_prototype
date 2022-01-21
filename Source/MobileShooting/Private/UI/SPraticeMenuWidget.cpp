// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPraticeMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USPraticeMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnPressed.AddDynamic(this, &USPraticeMenuWidget::OnResumeButtonPressed);
	ReturnMainMenuButton->OnPressed.AddDynamic(this, &USPraticeMenuWidget::OnReturnMainMenuButtonPressed);
}

void USPraticeMenuWidget::OnResumeButtonPressed()
{
	OnResumeDelegate.Broadcast();
}

void USPraticeMenuWidget::OnReturnMainMenuButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("GameStartLevel"));
}

