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


	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	virtual void BeginPlay() override;

	void MoveToPlayer();

	void AttackPlayer();
	
protected:
	ACharacter *player;

	ABase_Enemy *ownCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AActor> CharacterClass;

	bool Attacked;
};
