// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base_Class_Character.generated.h"

class UCameraComponent;

UCLASS()
class A_CURE_API ABase_Class_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Class_Character();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UCameraComponent *Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float DashSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);

	void Dash();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

