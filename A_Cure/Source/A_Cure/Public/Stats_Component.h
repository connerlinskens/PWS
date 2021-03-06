// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Stats_Component.generated.h"

// OnDamageTaken event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnDamageTakenSignature, UStats_Component*, StatsComp, float, Health, float, DamageTaken, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class A_CURE_API UStats_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStats_Component();

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetCurrentHealth();

	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetMaxHealth();

	float GetArmor();
	float GetAttackDamage();
	float GetAbilityDamage();
	FVector GetKnockBack();
	float GetSpeed();
	TSubclassOf<UDamageType> GetDamageType();
	float GetAttackRate();
	float GetRecoverRate();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Stats")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Armor;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float AttackDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float AbilityDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	FVector KnockBack;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Speed;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float AttackRate;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float RecoverRate;

	float CurrentHealth;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	TSubclassOf<UDamageType> DamageType;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDamageTakenSignature OnDamageTaken;
};
