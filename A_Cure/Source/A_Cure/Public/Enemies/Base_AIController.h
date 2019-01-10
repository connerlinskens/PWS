// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Base_AIController.generated.h"

class ABase_Character;

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
	
protected:
	ABase_Character *player;
};
