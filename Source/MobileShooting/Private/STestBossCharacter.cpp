// Fill out your copyright notice in the Description page of Project Settings.


#include "STestBossCharacter.h"
#include "Components/SHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SHPBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "SProjectile.h"
#include "AI/STestBossAIController.h"

// Sets default values
ASTestBossCharacter::ASTestBossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddUObject(this, &ASTestBossCharacter::OnHealthChanged);

	HPBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComp"));
	HPBarWidgetComp->SetupAttachment(GetMesh());
	HPBarWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	HPBarWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

	// AI
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	
}

// Called when the game starts or when spawned
void ASTestBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	auto AIController = Cast<ASTestBossAIController>(GetController());

	if (nullptr != AIController)
	{
		AIController->RunAI();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller is nullptr"));
	}
}

void ASTestBossCharacter::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UpdateHPBarWidget();
}

// Called every frame
void ASTestBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTestBossCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	HPBarWidgetComp->InitWidget();
	auto HPBarWidget = Cast<USHPBarWidget>(HPBarWidgetComp->GetUserWidgetObject());
	if (nullptr != HPBarWidget)
	{
		HPBarWidget->BindCharacterHealthComponent(HealthComp);
	}
}

void ASTestBossCharacter::UpdateHPBarWidget()
{
	auto HPBarWidget = Cast<USHPBarWidget>(HPBarWidgetComp->GetUserWidgetObject());
	if (nullptr == HPBarWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("HPBarWidget is nullptr"));
		return;
	}
	HPBarWidget->UpdateHPWidget();
}

void ASTestBossCharacter::NormalAttack()
{
	//UE_LOG(LogTemp, Log, TEXT("TODO : Boss Normal Attack"));

	if (nullptr == ProjectileClass)
	{
		return;
	}

	ASProjectile* Bullet = GetWorld()->SpawnActor<ASProjectile>(ProjectileClass,
		GetActorLocation() + GetActorForwardVector() * 100.0f,
		GetActorRotation());
}


