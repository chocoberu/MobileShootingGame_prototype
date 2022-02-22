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

void USPraticeMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ResumeButton->OnPressed.RemoveDynamic(this, &USPraticeMenuWidget::OnResumeButtonPressed);
	ReturnMainMenuButton->OnPressed.RemoveDynamic(this, &USPraticeMenuWidget::OnReturnMainMenuButtonPressed);
}

void USPraticeMenuWidget::OnResumeButtonPressed()
{
	OnResumeDelegate.Broadcast();
}

void USPraticeMenuWidget::OnReturnMainMenuButtonPressed()
{
	FInputModeGameAndUI Mode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(Mode);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("GameStartLevel"));
}

