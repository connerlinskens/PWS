// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Class_Character.h"
#include "Ranger_Class.generated.h"

class ABow;

/**
 * 
 */
UCLASS()
class A_CURE_API ARanger_Class : public ABase_Class_Character
{
	GENERATED_BODY()
	
public:
	ARanger_Class();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void ReleaseArrow();

	ABow *myBow;
};
