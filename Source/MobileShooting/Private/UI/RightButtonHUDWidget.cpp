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

	MainAttackButton->OnPressed.AddDynamic(this, &URightButtonHUDWidget::OnMainAttackPressed);
	MainAttackButton->OnReleased.AddDynamic(this, &URightButtonHUDWidget::OnMainAttackReleased);
	
	SubAttackButton->OnPressed.AddDynamic(this, &URightButtonHUDWidget::OnSubAttackPressed);
	SubAttackButton->OnReleased.AddDynamic(this, &URightButtonHUDWidget::OnSubAttackReleased);

	SkillAttackButton->OnPressed.AddDynamic(this, &URightButtonHUDWidget::OnSkillAttackPressed);
	SkillAttackButton->OnReleased.AddDynamic(this, &URightButtonHUDWidget::OnSkillAttackReleased);

	MainAttackButton->IsFocusable = false;
	MainAttackButton->PressMethod = EButtonPressMethod::ButtonPress;
	SubAttackButton->IsFocusable = false;
	SubAttackButton->PressMethod = EButtonPressMethod::ButtonPress;
	SkillAttackButton->IsFocusable = false;
	SkillAttackButton->PressMethod = EButtonPressMethod::ButtonPress;
	
	MenuButton->OnClickedMenuButtonDelegate.AddUObject(this, &URightButtonHUDWidget::OnMenuButtonPressed);

	MainWeaponCoolTimeAnimationPauseTime = 0.0f;
	SubWeaponCoolTimeAnimationPauseTime = 0.0f;
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

void URightButtonHUDWidget::OnSkillAttackPressed()
{
	if (nullptr == PlayerCharacter)
	{
		return;
	}
	PlayerCharacter->StartSkillAttack();
}

void URightButtonHUDWidget::OnSkillAttackReleased()
{
	if (nullptr == PlayerCharacter)
	{
		return;
	}
	PlayerCharacter->StopSkillAttack();
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
	MainWeaponWeakPtr->OnSkillAttackDelegate.AddUObject(this, &URightButtonHUDWidget::PlaySkillAttackCoolTime);

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

void URightButtonHUDWidget::SetVisibleWeaponStatusAnimation(bool NewValue)
{
	// NewValue == true 이면 Pause Widget Animation  
	if (true == NewValue)
	{
		if (true == IsAnimationPlaying(MainWeaponCoolTimeAnimation))
		{
			MainWeaponCoolTimeAnimationPauseTime = PauseAnimation(MainWeaponCoolTimeAnimation);
		}
		if (true == IsAnimationPlaying(SubWeaponCoolTimeAnimation))
		{
			SubWeaponCoolTimeAnimationPauseTime = PauseAnimation(SubWeaponCoolTimeAnimation);
		}
		if (true == IsAnimationPlaying(SkillAttackCoolTimeAnimation))
		{
			SkillAttackCoolTimeAnimationPauseTime = PauseAnimation(SkillAttackCoolTimeAnimation);
		}
	}
	// false 이면 Restart Widget Animation
	else
	{
		if (0.0f != MainWeaponCoolTimeAnimationPauseTime)
		{
			float PlaySpeed = 1.0f / MainWeaponWeakPtr->GetReloadTime();
			PlayAnimation(MainWeaponCoolTimeAnimation, MainWeaponCoolTimeAnimationPauseTime, 1, EUMGSequencePlayMode::Forward, PlaySpeed, false);
		}

		if (0.0f != SubWeaponCoolTimeAnimationPauseTime)
		{
			float PlaySpeed = 1.0f / SubWeaponWeakPtr->GetReloadTime();
			PlayAnimation(SubWeaponCoolTimeAnimation, SubWeaponCoolTimeAnimationPauseTime, 1, EUMGSequencePlayMode::Forward, PlaySpeed, false);
		}

		if (0.0f != SkillAttackCoolTimeAnimationPauseTime)
		{
			float PlaySpeed = 1.0f / MainWeaponWeakPtr->GetSkillCoolTime();
			PlayAnimation(SkillAttackCoolTimeAnimation, SkillAttackCoolTimeAnimationPauseTime, 1, EUMGSequencePlayMode::Forward, PlaySpeed, false);
		}

		MainWeaponCoolTimeAnimationPauseTime = 0.0f;
		SubWeaponCoolTimeAnimationPauseTime = 0.0f;
		SkillAttackCoolTimeAnimationPauseTime = 0.0f;
	}
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

	if (MainWeaponCount <= 0)
	{
		float PlaySpeed = 1.0f / MainWeaponWeakPtr->GetReloadTime();
		PlayAnimation(MainWeaponCoolTimeAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, PlaySpeed, false);
	}
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

	if (SubWeaponCount <= 0)
	{
		float PlaySpeed = 1.0f / SubWeaponWeakPtr->GetReloadTime();
		PlayAnimation(SubWeaponCoolTimeAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, PlaySpeed, false);
	}
}

void URightButtonHUDWidget::PlaySkillAttackCoolTime()
{
	if (false == MainWeaponWeakPtr.IsValid())
	{
		return;
	}

	float PlaySpeed = 1.0f / MainWeaponWeakPtr->GetSkillCoolTime();
	PlayAnimation(SkillAttackCoolTimeAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, PlaySpeed, false);
}

void URightButtonHUDWidget::StopAllWidgetAnimations()
{
	// StopAllAnimations()로 처리할 경우 WidgetAnimation의 Time이 0.0f로 고정되기 때문에 해당 방식으로 구현
	SetAnimationCurrentTime(MainWeaponCoolTimeAnimation, 1.0f);
	SetAnimationCurrentTime(SubWeaponCoolTimeAnimation, 1.0f);
	SetAnimationCurrentTime(SkillAttackCoolTimeAnimation, 1.0f);
}
