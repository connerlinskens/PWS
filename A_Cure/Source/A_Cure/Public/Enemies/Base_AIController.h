// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Base_AIController.generated.h"

class ACharacter;
class ABase_Enemy;
class ABase_Class_Character;

/**
 * 
 */
UCLASS()
class A_CURE_API ABase_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABase_AIController();

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	void MoveToPlayer();

	void AttackPlayer();

	void ResetAttack();
	
protected:
	ACharacter *player;

	ABase_Enemy *ownCharacter;

	TArray<AActor*> OverlappingActors;

	TArray<AActor*> AttackingActors;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AActor> CharacterClass;

	FTimerHandle AttackTimerHandle;

	bool Attacked;
};
