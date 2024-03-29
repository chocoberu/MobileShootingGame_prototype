// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SDummyCharacter.generated.h"

UCLASS()
class MOBILESHOOTING_API ASDummyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASDummyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(class USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void RestoreHealth();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* HPBarWidgetComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USDamageTextWidgetComponent> DamageTextWidgetCompClass;

	FTimerHandle RestoreHealthTimer;

	UPROPERTY(EditDefaultsOnly, Category = "DummyCharacter")
	float RestoreTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	void UpdateHPBarWidget(float Health);
};
