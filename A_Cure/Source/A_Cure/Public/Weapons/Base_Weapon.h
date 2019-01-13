// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_Weapon.generated.h"

class USkeletalMeshComponent;

enum WeaponType
{
	Melee,
	Ranged,
	None
};

UCLASS()
class A_CURE_API ABase_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABase_Weapon();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent *Root;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent *mesh;

	WeaponType weaponType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
