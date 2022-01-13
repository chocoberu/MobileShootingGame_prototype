// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SWeaponStatusWidget.h"
#include "Components/TextBlock.h"
#include "SCharacter.h"
#include "SWeapon.h"
#include "SSubWeapon.h"

void USWeaponStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void USWeaponStatusWidget::BindMainWeapon(ASWeapon* MainWeapon)
{
	if (nullptr == MainWeapon)
	{
		return;
	}

	MainWeaponWeakPtr = MainWeapon;
	MainWeaponWeakPtr->OnAttackDelegate.AddUObject(this, &USWeaponStatusWidget::SetMainWeaponText);

	SetMainWeaponText();
}

void USWeaponStatusWidget::BindSubWeapon(ASSubWeapon* SubWeapon)
{
	if (nullptr == SubWeapon)
	{
		return;
	}
	SubWeaponWeakPtr = SubWeapon;
	SubWeaponWeakPtr->OnAttackDelegate.AddUObject(this, &USWeaponStatusWidget::SetSubWeaponText);

	SetSubWeaponText();
}

void USWeaponStatusWidget::SetMainWeaponText()
{
	if (false == MainWeaponWeakPtr.IsValid())
	{
		MainWeaponStatusText->SetText(FText::FromString(TEXT("MainWeapon X")));
		return;
	}
	const int32 MainWeaponCount = MainWeaponWeakPtr->GetCurrentBulletCount();
	FString MainWeaponStr = TEXT("MainWeapon Bullet : ") + FString::FromInt(MainWeaponCount);

	MainWeaponStatusText->SetText(FText::FromString(MainWeaponStr));
}

void USWeaponStatusWidget::SetSubWeaponText()
{
	if (false == SubWeaponWeakPtr.IsValid())
	{
		SubWeaponStatusText->SetText(FText::FromString(TEXT("SubWeapon X")));
		return;
	}
	const int32 SubWeaponCount = SubWeaponWeakPtr->GetCurrentSubWeaponCount();
	FString SubWeaponStr = TEXT("SubWeapon Count : ") + FString::FromInt(SubWeaponCount);

	SubWeaponStatusText->SetText(FText::FromString(SubWeaponStr));
}