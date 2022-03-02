// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestWeaponSelectWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "SGameInstance.h"
#include "Kismet/GameplayStatics.h"

bool UTestWeaponSelectWidget::Initialize()
{
	if (false == Super::Initialize())
	{
		return false;
	}

	SelectButton->OnClicked.AddDynamic(this, &UTestWeaponSelectWidget::OnSelectButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UTestWeaponSelectWidget::OnCancelButtonClicked);

	UE_LOG(LogTemp, Log, TEXT("UTestWeaponSelectWidget Initialize"));
	return true;
}

void UTestWeaponSelectWidget::OnSelectButtonClicked()
{
	// TODO : EditableText에서 ID 값을 가져와서 SaveGame 
	// TODO : EditableText를 인벤토리로 변경

	auto TestGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == TestGameInstance)
	{
		return;
	}

	
	TestGameInstance->SetCurrentWeaponID(0);
	TestGameInstance->SetCurrentSubWeaponID(1001);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*TestGameInstance->GetCurrentSelectLevel()));
}

void UTestWeaponSelectWidget::OnCancelButtonClicked()
{
	//WeaponID->SetText(FText());
	//SubWeaponID->SetText(FText());

	OnCancelClickedDelegate.Broadcast();
}
