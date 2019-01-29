// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Base_Weapon.h"
#include "Bow.generated.h"

class AArrow;
/**
 * 
 */
UCLASS()
class A_CURE_API ABow : public ABase_Weapon
{
	GENERATED_BODY()
	
public:
	ABow();

	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;

	void ReleaseArrow();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	TSubclassOf<AActor> ArrowClass;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent *ArrowSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	FName ArrowSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	float MaxArrowSpeed;

	AArrow *SpawnedArrow;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	float LineTraceRange;

	bool tensioningBow;

	float ArrowSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	float ArrowTractionSpeed;

	bool bCanFire;

	FTimerHandle reloadTimerHandle;

protected:
	void Reload();

	virtual void BeginPlay() override;
};
