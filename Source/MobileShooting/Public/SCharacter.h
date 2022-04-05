// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "SCharacter.generated.h"

UCLASS()
class MOBILESHOOTING_API ASCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay(void) override;

	virtual void PostInitializeComponents() override;

	// Input Func
	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION()
	void OnHealthChanged(class USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float ArmLengthTo = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	FRotator ArmRotationTo = FRotator::ZeroRotator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float ArmLengthSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float ArmRotationSpeed;

	// Direction
	FVector DirectionToMove = FVector::ZeroVector;

	// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USCharacterAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* HPBarWidgetComp;

	// Weapon
	UPROPERTY()
	class ASWeapon* MainWeapon;

	UPROPERTY(EditAnywhere, Category = "Player")
	TSubclassOf<class ASWeapon> MainWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	UPROPERTY()
	class ASSubWeapon* SubWeapon;

	UPROPERTY(EditAnywhere, Category = "Player")
	TSubclassOf<class ASSubWeapon> SubWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName SubWeaponAttachSocketName;

	bool bIsAttack;

	// UI, HUD
	
	// Controller
	UPROPERTY()
	class ASPlayerController* PlayerController;
	
	// HP
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bDied;

	UPROPERTY(EditDefaultsOnly, Category = "HealthComponent")
	float RespawnTime;

	FTimerHandle RespawnTimer;

	// AI
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FGenericTeamId TeamId;

	// PlayerState
	UPROPERTY()
	class ASPlayerState* SPlayerState;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void StartMainAttack(void);

	virtual void StopMainAttack(void);

	virtual void ReloadMainWeapon(void);

	virtual void StartSubAttack(void);

	virtual void StopSubAttack(void);

	virtual void StartSkillAttack(void);

	virtual void StopSkillAttack(void);

	void RespawnCharacter(void);

	void UpdateHPBarWidget(void);

	class USHealthComponent* GetHealthComponent(void) { return HealthComp; }

	class ASWeapon* GetWeapon(void) { return MainWeapon; }

	class ASSubWeapon* GetSubWeapon(void) { return SubWeapon; }

	// TeamAgent
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
};
