// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "SWeapon.h"
#include "SSubWeapon.h"
#include "SGameInstance.h"
#include "SPlayerController.h"
#include "SPlayerState.h"
#include "SCharacterAnimInstance.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SHealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SHPBarWidget.h"
#include "Net/UnrealNetwork.h"

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

	// HealthComponent
	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddUObject(this, &ASCharacter::OnHealthChanged);
	//HealthComp->OnHealthChanged.AddUObject(this, &ASCharacter::Multicast_OnHealthChanged);
	HealthComp->SetNetAddressable();
	HealthComp->SetIsReplicated(true);

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

	// TEST CODE : Team 설정 관련
	TeamId = FGenericTeamId(0);

	bIsAttack = false;
	StartDirection = 1.0f;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("ASPlayerCharacter::BeginPlay() call"));

	if (GetActorForwardVector().X < 0.0f)
	{
		ArmRotationTo.Yaw = -180.0f;
		StartDirection = -1.0f;
	}

	//if (GetLocalRole() == ROLE_AutonomousProxy || GetLocalRole() == ROLE_Authority)
	if(true == IsLocallyControlled())
	{
		// TODO : 준비 완료 상태로 변경, GameState에서 모든 플레이어가 준비된 경우 CountDown 시작하도록 작성 필요
		PlayerController = Cast<ASPlayerController>(GetController());
		if (nullptr != PlayerController)
		{
			//PlayerController->Server_ReadyGame(true);
		}
	}
	
	AnimInstance = Cast<USCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (nullptr == AnimInstance)
	{
		//UE_LOG(LogTemp, Error, TEXT("AnimInstance is nullptr"));
	}

	PlayerController = Cast<ASPlayerController>(GetController());

	if (nullptr == PlayerController)
	{
		//UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr"));
	}

	SPlayerState = Cast<ASPlayerState>(GetPlayerState());
	if (nullptr == SPlayerState)
	{
		//UE_LOG(LogTemp, Error, TEXT("PlayerState is nullptr"));
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
	DirectionToMove.X = Value * StartDirection;
}

void ASCharacter::MoveRight(float Value)
{
	DirectionToMove.Y = Value * StartDirection;
}

void ASCharacter::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Multicast_OnHealthChanged(OwningHealthComp, Health, HealthDelta, DamageType, InstigatedBy, DamageCauser);
	/*UpdateHPBarWidget();

	if (Health <= 0.0f && !bDied)
	{
		OnCharacterDead(OwningHealthComp, Health, HealthDelta, DamageType, InstigatedBy, DamageCauser);
	}*/
}

void ASCharacter::Multicast_OnHealthChanged_Implementation(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("ASCharacter::Multicast_OnHealthChanged() called, Health : %f"), Health);
	UpdateHPBarWidget(Health);

	if (Health <= 0.0f && !bDied)
	{
		OnCharacterDead(OwningHealthComp, Health, HealthDelta, DamageType, InstigatedBy, DamageCauser);
	}
}

void ASCharacter::OnCharacterDead(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (nullptr != MainWeapon)
	{
		MainWeapon->StopNormalAttack();
	}

	// Ragdoll 처리 관련
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

	// Character를 5초 후에 시작 위치로 조정
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASCharacter::RespawnCharacter, RespawnTime, false);

	if (nullptr != SPlayerState)
	{
		SPlayerState->AddDeathScore();
	}
}

void ASCharacter::LoadWeapon()
{
	UE_LOG(LogTemp, Log, TEXT("ASCharacter::LoadWeapon() called"));
	int32 CurrentWeaponId = 0;
	int32 CurrentSubWeaponId = 1000;

	SPlayerState = Cast<ASPlayerState>(GetPlayerState());
	// TEST CODE
	// LoadWeapon() 시점에서 PlayerState가 생성되지 않은 경우
	if (nullptr == SPlayerState)
	{
		UE_LOG(LogTemp, Log, TEXT("ASCharacter::LoadWeapon(), SPlayerState is nullptr"));
		return;
	}

	CurrentWeaponId = SPlayerState->GetWeaponId();
	CurrentSubWeaponId = SPlayerState->GetSubWeaponId();

	UE_LOG(LogTemp, Log, TEXT("Current Weapon : %d Current SubWeapon : %d"), CurrentWeaponId, CurrentSubWeaponId);

	auto TestGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == TestGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("ASCharacter::LoadWeapon(), SGameInstance is nullptr"));
		return;
	}

	FSoftClassPath WeaponClassPath(TestGameInstance->GetWeaponPath(CurrentWeaponId) + TEXT("_C"));
	FSoftClassPath SubWeaponClassPath(TestGameInstance->GetSubWeaponPath(CurrentSubWeaponId) + TEXT("_C"));
	auto TestWeaponClass = WeaponClassPath.TryLoadClass<ASWeapon>();
	auto TestSubWeaponClass = SubWeaponClassPath.TryLoadClass<ASSubWeapon>();

	if (nullptr == TestWeaponClass || nullptr == TestSubWeaponClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ASCharacter::LoadWeapon(), WeaponClass is nullptr"));
		return;
	}

	// Spawn a default weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;

	MainWeapon = GetWorld()->SpawnActor<ASWeapon>(TestWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (nullptr != MainWeapon)
	{
		MainWeapon->SetOwner(this);
		MainWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		MainWeapon->SetOwnerAnimInstance(AnimInstance);

		MainWeapon->OnReloadMontageDelegate.AddUObject(this, &ASCharacter::Multicast_ReloadMainWeaon);
	}

	SubWeapon = GetWorld()->SpawnActor<ASSubWeapon>(TestSubWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (nullptr != SubWeapon)
	{
		SubWeapon->SetOwner(this);
		SubWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SubWeaponAttachSocketName);
	}

	// PlayerController Ready 처리
	if (GetNetMode() != ENetMode::NM_Standalone)
	{
		PlayerController = GetController<ASPlayerController>();
		if (nullptr == PlayerController)
		{
			return;
		}
		PlayerController->Server_ReadyGame(true);
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
		// TEST CODE
		if (false == bIsAttack)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
		}
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
	PlayerInputComponent->BindAction("SkillAttack", EInputEvent::IE_Pressed, this, &ASCharacter::StartSkillAttack);
	PlayerInputComponent->BindAction("SkillAttack", EInputEvent::IE_Released, this, &ASCharacter::StopSkillAttack);
}

void ASCharacter::StartMainAttack()
{
	if (false == IsLocallyControlled())
	{
		return;
	}
	
	Server_StartMainAttack();

	/*if (nullptr != MainWeapon && false == bDied && false == MainWeapon->IsReloading())
	{
		bIsAttack = true;
		//MainWeapon->StartNormalAttack();

		float FirstDelay = MainWeapon->GetFirstDelay();
		float NormalAttackCoolTime = MainWeapon->GetNormalAttackCoolTime();
		GetWorldTimerManager().SetTimer(NormalAttackTimer, this, &ASCharacter::MainAttack, NormalAttackCoolTime, true, FirstDelay);
	}*/
}

void ASCharacter::MainAttack()
{
	if (nullptr != MainWeapon && false == bDied && false == MainWeapon->IsReloading())
	{
		AnimInstance->PlayNormalAttack();
	}
}

void ASCharacter::StopMainAttack()
{
	if (false == IsLocallyControlled())
	{
		return;
	}

	Server_StopMainAttack();

}

void ASCharacter::Server_StartMainAttack_Implementation()
{
	if (nullptr != MainWeapon && false == bDied && false == MainWeapon->IsReloading())
	{
		bIsAttack = true;
		//MainWeapon->StartNormalAttack();

		float FirstDelay = MainWeapon->GetFirstDelay();
		float NormalAttackCoolTime = MainWeapon->GetNormalAttackCoolTime();
		GetWorldTimerManager().SetTimer(NormalAttackTimer, this, &ASCharacter::Multicast_MainAttack, NormalAttackCoolTime, true, FirstDelay);
	}
}

bool ASCharacter::Server_StartMainAttack_Validate()
{
	return true;
}

void ASCharacter::Multicast_MainAttack_Implementation()
{
	if (nullptr != MainWeapon && false == bDied && false == MainWeapon->IsReloading())
	{
		AnimInstance->PlayNormalAttack();
	}
}

void ASCharacter::Server_StopMainAttack_Implementation()
{
	if (nullptr != MainWeapon && false == bDied)
	{
		bIsAttack = false;
		GetWorldTimerManager().ClearTimer(NormalAttackTimer);
		//MainWeapon->StopNormalAttack();
	}
}

bool ASCharacter::Server_StopMainAttack_Validate()
{
	return true;
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

void ASCharacter::Multicast_ReloadMainWeaon_Implementation()
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
	if (false == IsLocallyControlled())
	{
		return;
	}

	if (nullptr != SubWeapon && false == bDied && false == HasAuthority())
	{
		SubWeapon->Client_StartSubWeaponAttack();
	}

	Server_StartSubAttack();
}

void ASCharacter::StopSubAttack(void)
{
	if (false == IsLocallyControlled())
	{
		return;
	}

	if (nullptr != SubWeapon && false == bDied && false == HasAuthority())
	{
		SubWeapon->Client_StopSubWeaponAttack();
	}

	Server_StopSubAttack();
}

void ASCharacter::Server_StartSubAttack_Implementation()
{
	if (nullptr != SubWeapon && false == bDied)
	{
		SubWeapon->StartSubWeaponAttack();
	}
}

bool ASCharacter::Server_StartSubAttack_Validate()
{
	return true;
}

void ASCharacter::Server_StopSubAttack_Implementation()
{
	if (nullptr != SubWeapon && false == bDied)
	{
		SubWeapon->StopSubWeaponAttack();
	}
}

bool ASCharacter::Server_StopSubAttack_Validate()
{
	return true;
}

void ASCharacter::StartSkillAttack(void)
{
	if (nullptr != MainWeapon && false == bDied)
	{
		MainWeapon->StartSkillAttack();
	}
}

void ASCharacter::StopSkillAttack(void)
{
	if (nullptr != MainWeapon && false == bDied)
	{
		MainWeapon->StopSkillAttack();
	}
}

void ASCharacter::RespawnCharacter(void)
{
	HealthComp->RestoreHealth();
	//AnimInstance->SetDeadAnim(false);

	// SCharacter를 StartSpot으로 이동시킴
	PlayerController = Cast<ASPlayerController>(GetController());
	if (nullptr != PlayerController && true == PlayerController->StartSpot.IsValid())
	{
		SetActorLocation(PlayerController->StartSpot->GetActorLocation());
	}
	
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	
	GetMesh()->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeTransform(FTransform(FRotator(0.0f, -90.0f, 0.0f), FVector(0.0f, 0.0f, -88.0f)));
	
	GetCharacterMovement()->SetDefaultMovementMode();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HPBarWidgetComp->SetHiddenInGame(false);

	UpdateHPBarWidget(HealthComp->GetDefaultHealth());

	bDied = false;
	
	// Respawn 할 때 필요한 작업
	MainWeapon->ReloadWeapon();
	SubWeapon->ReloadSubWeapon();
}

void ASCharacter::UpdateHPBarWidget(float Health)
{
	auto HPBarWidget = Cast<USHPBarWidget>(HPBarWidgetComp->GetUserWidgetObject());
	if (nullptr == HPBarWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("HPBarWidget is nullptr"));
		return;
	}
	HPBarWidget->UpdateHPWidget(Health);
}

void ASCharacter::OnRep_UpdateHPBarWidgetColor()
{
	USHPBarWidget* HPBarWidget = Cast<USHPBarWidget>(HPBarWidgetComp->GetUserWidgetObject());
	if (nullptr == HPBarWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("HPBarWidget is nullptr"));
		return;
	}

	if (true == IsLocallyControlled())
	{
		HPBarWidget->SetHPBarColor(FColor::Green);
	}
	else
	{
		if (0 == TeamId.GetId())
		{
			HPBarWidget->SetHPBarColor(FColor::Blue);
		}
		else if (1 == TeamId.GetId())
		{
			HPBarWidget->SetHPBarColor(FColor::Red);
		}
	}
}

void ASCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (NewTeamID != FGenericTeamId::NoTeam)
	{
		TeamId = NewTeamID;
	}

	OnRep_UpdateHPBarWidgetColor();
}

void ASCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacter, HealthComp);
	DOREPLIFETIME(ASCharacter, MainWeapon);
	DOREPLIFETIME(ASCharacter, SubWeapon);
	DOREPLIFETIME(ASCharacter, TeamId);
}