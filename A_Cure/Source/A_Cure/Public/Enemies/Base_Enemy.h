// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base_Enemy.generated.h"

class UBoxComponent;
class USphereComponent;
class UStats_Component;

UCLASS()
class A_CURE_API ABase_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent *hitCollider;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStats_Component *Stats;

	UCapsuleComponent *collider;
	UCharacterMovementComponent *MovementComp;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> NormalDamageType;

	UPROPERTY(VisibleAnywhere)
	USphereComponent *DetectionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float DetectionRange;

	bool bStunned;

private:
	UFUNCTION()
	void OnDamageTaken(UStats_Component* OwningStatsComp, float Health, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:
	// Array to store the arrows that have hit this enemy
	TArray<AActor*> ChildActors;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Getters and setters
	UStats_Component *GetStats();
	TSubclassOf<UDamageType> GetDamageType();
	UBoxComponent* GetHitCollider();
	USphereComponent* GetDetectionSphere();
	USkeletalMeshComponent *GetEnemyMesh();
	bool GetStunned();
	void SetStunned(bool newValue);


};
