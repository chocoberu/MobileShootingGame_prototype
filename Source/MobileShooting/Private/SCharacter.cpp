// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "SWeapon.h"
#include "SSubWeapon.h"
#include "Components/SHealthComponent.h"
#include "SCharacterAnimInstance.h"
#include "SPlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camera Component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->TargetArmLength = 300.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddUObject(this, &ASCharacter::OnHealthChanged);

	// UI

	WeaponAttachSocketName = TEXT("hand_r_weapon");
	SubWeaponAttachSocketName = TEXT("thigh_r_SubWeapon");
	bDied = false;

	// 카메라 설정
	{
		SpringArmComp->bUsePawnControlRotation = false;
		SpringArmComp->bInheritPitch = false;
		SpringArmComp->bInheritRoll = false;
		SpringArmComp->bInheritYaw = false;
		SpringArmComp->bDoCollisionTest = false;
		bUseControllerRotationYaw = true;
	}

	RespawnTime = 5.0f;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance = Cast<USCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (nullptr == AnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("AnimInstance is nullptr"));
	}

	PlayerController = Cast<ASPlayerController>(GetController());

	if (nullptr == PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr"));
	}

	// Spawn a default weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;

	MainWeapon = GetWorld()->SpawnActor<ASWeapon>(MainWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (nullptr != MainWeapon)
	{
		MainWeapon->SetOwner(this);
		MainWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);

		MainWeapon->OnReloadMontageDelegate.AddUObject(this, &ASCharacter::ReloadMainWeapon);
	}

	SubWeapon = GetWorld()->SpawnActor<ASSubWeapon>(SubWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (nullptr != SubWeapon)
	{
		SubWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SubWeaponAttachSocketName);
	}
}

void ASCharacter::MoveForward(float Value)
{
	DirectionToMove.X = Value;
}

void ASCharacter::MoveRight(float Value)
{
	DirectionToMove.Y = Value;
}

void ASCharacter::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		if (nullptr != MainWeapon)
		{
			MainWeapon->StopNormalAttack();
			//MainWeapon->SetLifeSpan(10.0f);
		}

		if (nullptr != AnimInstance)
		{
			AnimInstance->SetDeadAnim(true);
		}

		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// TODO : Character를 5초 후에 리스폰 or 시작 위치로 조정

		GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASCharacter::RespawnCharacter, RespawnTime, false);

	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);
	SpringArmComp->SetRelativeRotation(FMath::RInterpTo(SpringArmComp->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
	if (DirectionToMove.SizeSquared() > 0.0f)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
		AddMovementInput(DirectionToMove);
	}
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("MainAttack", EInputEvent::IE_Pressed, this, &ASCharacter::StartMainAttack);
	PlayerInputComponent->BindAction("MainAttack", EInputEvent::IE_Released, this, &ASCharacter::StopMainAttack);
	PlayerInputComponent->BindAction("SubAttack", EInputEvent::IE_Pressed, this, &ASCharacter::StartSubAttack);
	PlayerInputComponent->BindAction("SubAttack", EInputEvent::IE_Released, this, &ASCharacter::StopSubAttack);

}

void ASCharacter::StartMainAttack()
{
	if (nullptr != MainWeapon && false == bDied)
	{
		AnimInstance->PlayNormalAttack();
		MainWeapon->StartNormalAttack();
	}
}

void ASCharacter::StopMainAttack()
{
	if (nullptr != MainWeapon && false == bDied)
	{
		MainWeapon->StopNormalAttack();
	}
}

void ASCharacter::ReloadMainWeapon()
{
	if (nullptr == MainWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("MainWeapon is nullptr"));
		return;
	}

	if (nullptr == AnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("AnimInstance is nullptr"));
		return;
	}

	AnimInstance->PlayReload();
}

void ASCharacter::StartSubAttack(void)
{
	if (nullptr != SubWeapon && false == bDied)
	{
		SubWeapon->StartSubWeaponAttack();
	}
}

void ASCharacter::StopSubAttack(void)
{
	if (nullptr != SubWeapon && false == bDied)
	{
		SubWeapon->StopSubWeaponAttack();
	}
}

void ASCharacter::RespawnCharacter(void)
{
	HealthComp->RestoreHealth();
	AnimInstance->SetDeadAnim(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 임시로 설정, TODO : 수정 필요

	bDied = false;
	// TODO : Respawn 할 때 필요한 작업 추가
}
