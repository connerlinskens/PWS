// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base_Class_Character.generated.h"

class UCameraComponent;
class UStats_Component;

UCLASS()
class A_CURE_API ABase_Class_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Class_Character();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	UStats_Component *GetStats();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStats_Component *Stats;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashSpeed;

	bool Dashed;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> NormalDamageType;

	FTimerHandle DashTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashDelay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);

	void Dash();

	void Damage();

	UFUNCTION()
	void OnDamageTaken(UStats_Component* OwningStatsComp, float Health, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void SetDashFalse();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

