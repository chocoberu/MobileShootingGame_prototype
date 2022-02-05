// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STestBossCharacter.generated.h"

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	E_Phase1 = 0 UMETA(DisplayName = "Phase1"),
	E_Phase2 UMETA(DisplayName = "Phase2"),
	E_Phase3 UMETA(DisplayName = "Phase3"),
};

UCLASS()
class MOBILESHOOTING_API ASTestBossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTestBossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(class USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* HPBarWidgetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Meta = (AllowPrivateAccess = true))
	class UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	class UAISenseConfig_Sight* SightConfig;

	// Weapon
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class ASProjectile> ProjectileClass;
	
	// BossPhase
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
	float Phase2Percent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
	float Phase3Percent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	void UpdateHPBarWidget(void);

	void NormalAttack(void);
};
