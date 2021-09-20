// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RightButtonHUDWidget.h"
#include "Components/Button.h"
#include "SPlayerController.h"
#include "SCharacter.h"

void URightButtonHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<ASPlayerController>(GetOwningPlayer());
	PlayerCharacter = Cast<ASCharacter>(GetOwningPlayerPawn());

	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("RightButtonHUDWidget : PlayerController is nullptr"));
	}

	MainAttackButton = Cast<UButton>(GetWidgetFromName(TEXT("MainAttack")));
	if (MainAttackButton != nullptr)
	{
		MainAttackButton->OnPressed.AddDynamic(this, &URightButtonHUDWidget::OnMainAttackPressed);
		MainAttackButton->OnReleased.AddDynamic(this, &URightButtonHUDWidget::OnMainAttackReleased);
	}
}

void URightButtonHUDWidget::OnMainAttackPressed()
{
	if (PlayerCharacter == nullptr)
	{
		return;
	}
	PlayerCharacter->StartMainAttack();
}

void URightButtonHUDWidget::OnMainAttackReleased()
{
	if (PlayerCharacter == nullptr)
	{
		return;
	}
	PlayerCharacter->StopMainAttack();
}