// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "SWeapon.h"
#include "Components/SHealthComponent.h"
#include "SCharacterAnimInstance.h"

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

	WeaponAttachSocketName = TEXT("hand_r_weapon");
	bDied = false;

	// Ä«¸Þ¶ó
	{
		//ArmLengthTo = 800.0f;
		//ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		SpringArmComp->bUsePawnControlRotation = false;
		SpringArmComp->bInheritPitch = false;
		SpringArmComp->bInheritRoll = false;
		SpringArmComp->bInheritYaw = false;
		SpringArmComp->bDoCollisionTest = false;
		bUseControllerRotationYaw = true;
	}
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance = Cast<USCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance == nullptr)
		UE_LOG(LogTemp, Error, TEXT("AnimInstance is nullptr"));

	// Spawn a default weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	MainWeapon = GetWorld()->SpawnActor<ASWeapon>(MainWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (MainWeapon != nullptr)
	{
		MainWeapon->SetOwner(this);
		MainWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);

		MainWeapon->OnReloadMontageDelegate.AddUObject(this, &ASCharacter::ReloadMainWeapon);
	}
}

void ASCharacter::MoveForward(float Value)
{
	//AddMovementInput(GetActorForwardVector() * Value);

	DirectionToMove.X = Value;
}

void ASCharacter::MoveRight(float Value)
{
	//AddMovementInput(GetActorRightVector() * Value);
	DirectionToMove.Y = Value;
}

void ASCharacter::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		if (MainWeapon != nullptr)
		{
			MainWeapon->StopNormalAttack();
		}

		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
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
	//PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("MainAttack", EInputEvent::IE_Pressed, this, &ASCharacter::StartMainAttack);
	PlayerInputComponent->BindAction("MainAttack", EInputEvent::IE_Released, this, &ASCharacter::StopMainAttack);

}

void ASCharacter::StartMainAttack()
{
	if (MainWeapon != nullptr && bDied == false)
	{
		AnimInstance->PlayNormalAttack();
		MainWeapon->StartNormalAttack();
	}
}

void ASCharacter::StopMainAttack()
{
	if (MainWeapon != nullptr && bDied == false)
	{
		MainWeapon->StopNormalAttack();
	}
}

void ASCharacter::ReloadMainWeapon()
{
	if (MainWeapon == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MainWeapon is nullptr"));
		return;
	}

	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AnimInstance is nullptr"));
		return;
	}

	AnimInstance->PlayReload();
}
