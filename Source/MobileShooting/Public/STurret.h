// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STurret.generated.h"

UCLASS()
class MOBILESHOOTING_API ASTurret : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TurretAttack();

protected:
	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Meta = (AllowPrivateAccess = true))
	class UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleDefaultsOnly, Category = "Turret")
	FName WeaponMuzzleSocketName;

	UPROPERTY(EditAnywhere, Category = "Turret")
	TSubclassOf<class ASProjectile> TurretProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	float TurretLifeSpan;

	UPROPERTY(BlueprintReadOnly, Category = "Turret")
	bool bDied;

	UFUNCTION()
	void OnHealthChanged(class USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
};
