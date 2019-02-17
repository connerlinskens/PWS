// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Base_Weapon.h"
#include "Bow.generated.h"

class AArrow;
class UCameraShake;
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
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AActor> ArrowClass;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent *ArrowSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName ArrowSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float MaxArrowSpeed;

	AArrow *SpawnedArrow;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float LineTraceRange;

	bool tensioningBow;

	float ArrowSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ArrowTractionSpeed;

	bool bCanFire;

	FTimerHandle reloadTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> FireCamShake;

	FTimerHandle FireShakeTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireShakeDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TensoningBowSpeed;

	float DefaultMovementSpeed;

protected:
	void Reload();

	void FireShake();
};
