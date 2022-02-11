// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"

using FOnHealthChangedDelegate = TMulticastDelegate<void(class USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)>;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOBILESHOOTING_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void UpdateDamageText(const float Damage);

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
		AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, Category = "HealthComponent")
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "HealthComponent")
	float DefaultHealth;

	bool bIsDead;

	UPROPERTY(EditDefaultsOnly, Category = "HealthComponent")
	bool bUseDamageText;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USDamageTextWidgetComponent> DamageTextWidgetCompClass;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetHealth() const;
	float GetHPRatio() const;
	float GetDefaultHealth() const;

	void SetHealth(float NewHealth);
	void RestoreHealth(void);

	FOnHealthChangedDelegate OnHealthChanged;
		
};
