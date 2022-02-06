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
#include "UI/SHPBarWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	SubWeaponAttachSocketName = TEXT("thigh_r_SubWeapon");
	bDied = false;

	// UI
	// HP Bar
	HPBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComp"));
	HPBarWidgetComp->SetupAttachment(GetMesh());
	HPBarWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	HPBarWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

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
		MainWeapon->SetOwnerAnimInstance(AnimInstance);

		MainWeapon->OnReloadMontageDelegate.AddUObject(this, &ASCharacter::ReloadMainWeapon);
	}

	SubWeapon = GetWorld()->SpawnActor<ASSubWeapon>(SubWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (nullptr != SubWeapon)
	{
		SubWeapon->SetOwner(this);
		SubWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SubWeaponAttachSocketName);
	}
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<USCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (nullptr == AnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("AnimInstance is nullptr"));
	}

	HPBarWidgetComp->InitWidget();
	auto HPBarWidget = Cast<USHPBarWidget>(HPBarWidgetComp->GetUserWidgetObject());
	if (nullptr != HPBarWidget)
	{
		HPBarWidget->BindCharacterHealthComponent(HealthComp);
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
	UpdateHPBarWidget();

	if (Health <= 0.0f && !bDied)
	{
		if (nullptr != MainWeapon)
		{
			MainWeapon->StopNormalAttack();
		}

		FVector ImpulseDireciton;
		if (nullptr != DamageCauser)
		{
			UE_LOG(LogTemp, Log, TEXT("Damage Causer : %s"), *DamageCauser->GetName());
			ImpulseDireciton = DamageCauser->GetActorForwardVector();
			ImpulseDireciton.Normalize();
		}
		else
		{
			ImpulseDireciton = -GetActorForwardVector();
			ImpulseDireciton.Normalize();
		}

		ImpulseDireciton = ImpulseDireciton * 1500.0f;

		bDied = true;
		if (true == GetMesh()->IsUsingAbsoluteLocation())
		{
			UE_LOG(LogTemp, Log, TEXT("Using Absolute Location"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Using Relative Location"));
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
		GetCharacterMovement()->DisableMovement();

		GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
		GetMesh()->AddImpulseToAllBodiesBelow(ImpulseDireciton, NAME_None);

		//GetMovementComponent()->StopMovementImmediately();
		HPBarWidgetComp->SetHiddenInGame(true);

		// Weapon Reload 관련 처리 
		PlayerController = Cast<ASPlayerController>(GetController());
		if (nullptr != PlayerController)
		{
			PlayerController->OnPlayerDead();
			MainWeapon->StopReloadWeapon();
			SubWeapon->StopReloadSubWeapon();
		}

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

	//UE_LOG(LogTemp, Log, TEXT("DirectionToMove : %s"), *DirectionToMove.ToString());
	if (DirectionToMove.SizeSquared() > 0.0f)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
		//UE_LOG(LogTemp, Log, TEXT("FRotationMatrix::MakeFromX(DirectionToMove) : %s"), *FRotationMatrix::MakeFromX(DirectionToMove).Rotator().ToString());
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
	if (nullptr != MainWeapon && false == bDied && false == MainWeapon->IsReloading())
	{
		MainWeapon->StartNormalAttack();
	}
}

void ASCharacter::StopMainAttack()
{
	if (nullptr != MainWeapon && false == bDied)
	{
		//GetWorldTimerManager().ClearTimer(NormalAttackTimer);
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
	//AnimInstance->SetDeadAnim(false);
	
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	
	GetMesh()->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeTransform(FTransform(FRotator(0.0f, -90.0f, 0.0f), FVector(0.0f, 0.0f, -88.0f)));
	
	GetCharacterMovement()->SetDefaultMovementMode();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HPBarWidgetComp->SetHiddenInGame(false);

	UpdateHPBarWidget();

	bDied = false;
	// TODO : Respawn 할 때 필요한 작업 추가
	
	MainWeapon->ReloadWeapon();
	SubWeapon->ReloadSubWeapon();
}

void ASCharacter::UpdateHPBarWidget()
{
	auto HPBarWidget = Cast<USHPBarWidget>(HPBarWidgetComp->GetUserWidgetObject());
	if (nullptr == HPBarWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("HPBarWidget is nullptr"));
		return;
	}
	HPBarWidget->UpdateHPWidget();
}
