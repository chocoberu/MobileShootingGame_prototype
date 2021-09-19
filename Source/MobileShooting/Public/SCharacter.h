// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class MOBILESHOOTING_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	// Weapon
	UPROPERTY()
	class ASWeapon* MainWeapon;

	UPROPERTY(EditAnywhere, Category = "Player")
	TSubclassOf<class ASWeapon> MainWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bDied;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void StartMainAttack();

	virtual void StopMainAttack();

	virtual void ReloadMainWeapon();

};
