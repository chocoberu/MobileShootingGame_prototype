// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SDamageTextWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void SetDamageText(float Damage);
};
