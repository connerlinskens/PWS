// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class ABase_Class_Character;

UCLASS()
class A_CURE_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

	void Launch(float Speed);

	void SetOwner(ABase_Class_Character *Owner);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent *Root;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent *ArrowBase;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent *HitBox;

	UProjectileMovementComponent *MovementComp;

	UPROPERTY(EditDefaultsOnly, Category = "noice")
	float GravityStrength;

	UFUNCTION()
	void FreezeArrow(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	ABase_Class_Character *myOwner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
