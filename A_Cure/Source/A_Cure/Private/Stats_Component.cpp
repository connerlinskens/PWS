// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Stats_Component.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UStats_Component::UStats_Component()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Setting the defaults of the viriables
	MaxHealth = 100.f;
	Armor = 10.f;
	AttackDamage = 20.f;
	AbilityDamage = 40.f;
	KnockBack = FVector(2000.f);
	Speed = 400.f;
}

// Getters
float UStats_Component::GetCurrentHealth() { return CurrentHealth; }
float UStats_Component::GetMaxHealth() { return MaxHealth; }
float UStats_Component::GetArmor() { return Armor; }
float UStats_Component::GetAttackDamage() { return AttackDamage; }
float UStats_Component::GetAbilityDamage() { return AbilityDamage; }
FVector UStats_Component::GetKnockBack() { return KnockBack; }
float UStats_Component::GetSpeed() { return Speed; }


// Called when the game starts
void UStats_Component::BeginPlay()
{
	Super::BeginPlay();

	// Setting the current health to start with the max health
	CurrentHealth = MaxHealth;

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UStats_Component::HandleTakeAnyDamage);
	}
}

void UStats_Component::HandleTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	float ActualDamage = Damage;
	ActualDamage -= Armor;

	if (ActualDamage <= 0.0f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("Health Changed by: %s, is now: %s"), *FString::SanitizeFloat(ActualDamage), *FString::SanitizeFloat(CurrentHealth));

	OnDamageTaken.Broadcast(this, CurrentHealth, ActualDamage, DamageType, InstigatedBy, DamageCauser);
}

