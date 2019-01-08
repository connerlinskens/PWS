// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStats_Component;

UCLASS()
class A_CURE_API ABase_Character : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent *CameraArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStats_Component *StatsComponent;

	bool bIsInLockMode;

	UPROPERTY(EditDefaultsOnly, Category="Stats")
	TSubclassOf<UDamageType> NormalDamageType;

public:
	// Sets default values for this character's properties
	ABase_Character();

	UFUNCTION(BlueprintCallable, Category="Stats")
	UStats_Component *GetStats();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);

	void Damage();

	UFUNCTION()
	void OnDamageTaken(UStats_Component* OwningStatsComp, float Health, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
