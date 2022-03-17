// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHealthComponent.h"
#include "UI/SDamageTextWidgetComponent.h"

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DefaultHealth = 100.0f;

	static ConstructorHelpers::FClassFinder<USDamageTextWidgetComponent> WC_DamageText(TEXT("/Game/UI/WC_DamageTextComponent.WC_DamageTextComponent_C"));
	if (true == WC_DamageText.Succeeded())
	{
		DamageTextWidgetCompClass = WC_DamageText.Class;
	}
	bUseDamageText = true;
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();

	if (Owner != nullptr)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
	}

	RestoreHealth();
}

void USHealthComponent::UpdateDamageText(const float Damage)
{
	// DamageText Widget Update
	if (nullptr == DamageTextWidgetCompClass)
	{
		return;
	}
	auto DamageTextWidgetComp = NewObject<USDamageTextWidgetComponent>(this, DamageTextWidgetCompClass);
	//auto DamageTextWidgetComp = Cast<USDamageTextWidgetComponent>(AddComponentByClass(DamageTextWidgetCompClass, true, GetActorTransform(), false));
	if (nullptr == DamageTextWidgetComp)
	{
		UE_LOG(LogTemp, Error, TEXT("%s DamageTextWidgetComponent is nullptr"), *GetName());
		return;
	}
	DamageTextWidgetComp->RegisterComponent();
	DamageTextWidgetComp->InitWidget();

	float RandomValue = FMath::RandRange(-30.0f, 30.0f);
	DamageTextWidgetComp->SetRelativeLocation(GetOwner()->GetActorLocation() + FVector(RandomValue, RandomValue, 0.0f));
	DamageTextWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	DamageTextWidgetComp->SetDamageText(Damage);

}

void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsDead == true)
	{
		return;
	}
	// TODO : 같은 팀에게 받은 데미지 무시

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	UE_LOG(LogTemp, Log, TEXT("Health Changed : %s"), *FString::SanitizeFloat(Health));

	bIsDead = Health <= 0.0f;

	if (true == bUseDamageText)
	{
		UpdateDamageText(Damage);
	}
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	/*if (bIsDead == true)
	{
		// TODO : 처리
	}*/
}


// Called every frame
void USHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float USHealthComponent::GetHealth() const
{
	return Health;;
}

float USHealthComponent::GetHPRatio() const
{
	return Health < KINDA_SMALL_NUMBER ? 0.0f : (Health / DefaultHealth);
}

float USHealthComponent::GetDefaultHealth() const
{
	return DefaultHealth;;
}

void USHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, DefaultHealth);
	OnHealthChanged.Broadcast(this, Health, 0.0f, nullptr, nullptr, nullptr);
}

void USHealthComponent::RestoreHealth(void)
{
	UE_LOG(LogTemp, Log, TEXT("%s Restore Health"), *GetOwner()->GetName());
	Health = DefaultHealth;
	bIsDead = false;
}
