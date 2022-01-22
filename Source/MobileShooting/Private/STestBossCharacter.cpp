// Fill out your copyright notice in the Description page of Project Settings.


#include "STestBossCharacter.h"
#include "Components/SHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SHPBarWidget.h"
#include "Components/WidgetComponent.h"

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
	
}

// Called when the game starts or when spawned
void ASTestBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASTestBossCharacter::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

}

// Called every frame
void ASTestBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTestBossCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}


