// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RightButtonHUDWidget.h"
#include "UI/SInGameMenuButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SPlayerController.h"
#include "SCharacter.h"
#include "SWeapon.h"
#include "SSubWeapon.h"

void URightButtonHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<ASPlayerController>(GetOwningPlayer());
	PlayerCharacter = Cast<ASCharacter>(GetOwningPlayerPawn());

	if (nullptr == PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("RightButtonHUDWidget : PlayerController is nullptr"));
		return;
	}

	MainAttackButton = Cast<UButton>(GetWidgetFromName(TEXT("MainAttack")));
	if (nullptr != MainAttackButton)
	{
		MainAttackButton->OnPressed.AddDynamic(this, &URightButtonHUDWidget::OnMainAttackPressed);
		MainAttackButton->OnReleased.AddDynamic(this, &URightButtonHUDWidget::OnMainAttackReleased);
	}
	SubAttackButton = Cast<UButton>(GetWidgetFromName(TEXT("SubAttack")));
	if (nullptr != SubAttackButton)
	{
		SubAttackButton->OnPressed.AddDynamic(this, &URightButtonHUDWidget::OnSubAttackPressed);
		SubAttackButton->OnReleased.AddDynamic(this, &URightButtonHUDWidget::OnSubAttackReleased);
	}

	MenuButton->OnClickedMenuButtonDelegate.AddUObject(this, &URightButtonHUDWidget::OnMenuButtonPressed);
}

void URightButtonHUDWidget::OnMainAttackPressed()
{
	if (nullptr == PlayerCharacter)
	{
		return;
	}
	PlayerCharacter->StartMainAttack();
}

void URightButtonHUDWidget::OnMainAttackReleased()
{
	if (nullptr == PlayerCharacter)
	{
		return;
	}
	PlayerCharacter->StopMainAttack();
}

void URightButtonHUDWidget::OnSubAttackPressed()
{
	if (nullptr == PlayerCharacter)
	{
		return;
	}
	PlayerCharacter->StartSubAttack();
}

void URightButtonHUDWidget::OnSubAttackReleased()
{
	if (nullptr == PlayerCharacter)
	{
		return;
	}
	PlayerCharacter->StopSubAttack();
}

void URightButtonHUDWidget::OnMenuButtonPressed()
{
	PlayerController->OnGamePause();
}

void URightButtonHUDWidget::SetHiddenMenuButton(bool NewValue)
{
	MenuButton->SetHiddenWidget(NewValue);
}

void URightButtonHUDWidget::BindMainWeapon(ASWeapon* MainWeapon)
{
	if (nullptr == MainWeapon)
	{
		return;
	}

	MainWeaponWeakPtr = MainWeapon;
	MainWeaponWeakPtr->OnAttackDelegate.AddUObject(this, &URightButtonHUDWidget::SetMainWeaponText);

	SetMainWeaponText();
}

void URightButtonHUDWidget::BindSubWeapon(ASSubWeapon* SubWeapon)
{
	if (nullptr == SubWeapon)
	{
		return;
	}
	SubWeaponWeakPtr = SubWeapon;
	SubWeaponWeakPtr->OnAttackDelegate.AddUObject(this, &URightButtonHUDWidget::SetSubWeaponText);

	SetSubWeaponText();
}

void URightButtonHUDWidget::SetMainWeaponText()
{
	if (false == MainWeaponWeakPtr.IsValid())
	{
		MainWeaponStatus->SetText(FText::FromString(TEXT("X")));
		return;
	}
	const int32 MainWeaponCount = MainWeaponWeakPtr->GetCurrentBulletCount();
	FString MainWeaponStr = FString::FromInt(MainWeaponCount);

	MainWeaponStatus->SetText(FText::FromString(MainWeaponStr));
}

void URightButtonHUDWidget::SetSubWeaponText()
{
	if (false == SubWeaponWeakPtr.IsValid())
	{
		SubWeaponStatus->SetText(FText::FromString(TEXT("X")));
		return;
	}
	const int32 SubWeaponCount = SubWeaponWeakPtr->GetCurrentSubWeaponCount();
	FString SubWeaponStr = FString::FromInt(SubWeaponCount);

	SubWeaponStatus->SetText(FText::FromString(SubWeaponStr));
}
