// Fill out your copyright notice in the Description page of Project Settings.


#include "SDummyCharacter.h"
#include "Components/SHealthComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/SHPBarWidget.h"
#include "UI/SDamageTextWidgetComponent.h"

// Sets default values
ASDummyCharacter::ASDummyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddUObject(this, &ASDummyCharacter::OnHealthChanged);

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	HPBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComp"));
	HPBarWidgetComp->SetupAttachment(GetMesh());
	HPBarWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	HPBarWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

}

// Called when the game starts or when spawned
void ASDummyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASDummyCharacter::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// HP Bar Update
	auto HPBarWidget = Cast<USHPBarWidget>(HPBarWidgetComp->GetUserWidgetObject());
	if (nullptr == HPBarWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("HPBarWidget is nullptr"));
		return;
	}
	HPBarWidget->UpdateHPWidget();

	// DamageText Widget Update
	if (nullptr == DamageTextWidgetCompClass)
	{
		return;
	}
	auto DamageTextWidgetComp = Cast<USDamageTextWidgetComponent>(AddComponentByClass(DamageTextWidgetCompClass, true, GetActorTransform(), false));
	if (nullptr == DamageTextWidgetComp)
	{
		UE_LOG(LogTemp, Error, TEXT("%s DamageTextWidgetComponent is nullptr"), *GetName());
		return;
	}
	DamageTextWidgetComp->SetupAttachment(GetMesh());
	float RandomValue = FMath::RandRange(-50.0f, 50.0f);
	DamageTextWidgetComp->SetRelativeLocation(DamageTextWidgetComp->GetRelativeLocation() + FVector(RandomValue, 0.0f, RandomValue));
	DamageTextWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	DamageTextWidgetComp->SetDamageText(HealthDelta);

	// Restore Health Set
	if (true == GetWorldTimerManager().IsTimerActive(RestoreHealthTimer))
	{
		GetWorldTimerManager().ClearTimer(RestoreHealthTimer);
	}

	GetWorldTimerManager().SetTimer(RestoreHealthTimer, this, &ASDummyCharacter::RestoreHealth, RestoreTime, false);
}

void ASDummyCharacter::RestoreHealth()
{
	HealthComp->RestoreHealth();

	auto HPBarWidget = Cast<USHPBarWidget>(HPBarWidgetComp->GetUserWidgetObject());
	if (nullptr == HPBarWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("HPBarWidget is nullptr"));
		return;
	}
	HPBarWidget->UpdateHPWidget();
}

// Called every frame
void ASDummyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASDummyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	HPBarWidgetComp->InitWidget();
	auto HPBarWidget = Cast<USHPBarWidget>(HPBarWidgetComp->GetUserWidgetObject());
	if (nullptr != HPBarWidget)
	{
		HPBarWidget->BindCharacterHealthComponent(HealthComp);
	}
}

