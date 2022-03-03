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
	// TODO : EditableText���� ID ���� �����ͼ� SaveGame 
	// TODO : EditableText�� �κ��丮�� ����

	auto TestGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == TestGameInstance)
	{
		return;
	}

	// TEMP : ���� �ӽð� �ϵ��ڵ�
	TestGameInstance->SetCurrentWeaponID(0);
	TestGameInstance->SetCurrentSubWeaponID(1002);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*TestGameInstance->GetCurrentSelectLevel()));
}

void UTestWeaponSelectWidget::OnCancelButtonClicked()
{
	//WeaponID->SetText(FText());
	//SubWeaponID->SetText(FText());

	OnCancelClickedDelegate.Broadcast();
}
