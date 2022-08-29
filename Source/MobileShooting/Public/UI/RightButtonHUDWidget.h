// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RightButtonHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API URightButtonHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	class ASWeapon* MainWeapon;

	UPROPERTY()
	class ASSubWeapon* SubWeapon;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* MainAttackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SubAttackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SkillAttackButton;

	UPROPERTY(meta = (BindWidget))
	class USInGameMenuButtonWidget* MenuButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MainWeaponStatus;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SubWeaponStatus;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillStatus;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* SubWeaponProgressBar;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SubWeaponCoolTimeAnimation;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MainWeaponProgressBar;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* MainWeaponCoolTimeAnimation;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* SkillAttackCoolTimeAnimation;
	
	UFUNCTION()
	void OnMainAttackPressed();

	UFUNCTION()
	void OnMainAttackReleased();

	UFUNCTION()
	void OnSubAttackPressed();

	UFUNCTION()
	void OnSubAttackReleased();

	UFUNCTION()
	void OnSkillAttackPressed();

	UFUNCTION()
	void OnSkillAttackReleased();

	UFUNCTION()
	void OnMenuButtonPressed();

	UPROPERTY()
	class ASPlayerController* PlayerController;

	UPROPERTY()
	class ASCharacter* PlayerCharacter;

	float MainWeaponCoolTimeAnimationPauseTime;

	float SubWeaponCoolTimeAnimationPauseTime;

	float SkillAttackCoolTimeAnimationPauseTime;

public:

	void SetHiddenMenuButton(bool NewValue);

	void BindMainWeapon(class ASWeapon* NewMainWeapon);

	void BindSubWeapon(class ASSubWeapon* NewSubWeapon);

	void SetVisibleWeaponStatusAnimation(bool NewValue);

	UFUNCTION()
	void SetMainWeaponText();

	UFUNCTION()
	void SetSubWeaponText();

	UFUNCTION()
	void PlaySkillAttackCoolTime();

	void StopAllWidgetAnimations();
};
