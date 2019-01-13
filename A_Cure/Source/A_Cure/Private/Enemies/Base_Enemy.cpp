// Fill out your copyright notice in the Description page of Project Settings.

#include "public/Enemies/Base_Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/Stats_Component.h"
#include "Public/Enemies/Base_AIController.h"

// Sets default values
ABase_Enemy::ABase_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setting the defaults values of the variables
	collider = GetCapsuleComponent();
	MovementComp = GetCharacterMovement();
	DetectionRange = 1000.f;

	// Setting op the hitbox
	hitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Box"));
	hitCollider->SetupAttachment(RootComponent);

	Stats = CreateDefaultSubobject<UStats_Component>(TEXT("Stats Component"));

	DetectionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Radius"));
	DetectionRadius->SetupAttachment(RootComponent);
	DetectionRadius->SetSphereRadius(DetectionRange);

	// Let the pawn get auto possessed by an AI controller
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Getters
UStats_Component *ABase_Enemy::GetStats() { return Stats; }
TSubclassOf<UDamageType> ABase_Enemy::GetDamageType() { return NormalDamageType; }
UBoxComponent *ABase_Enemy::GetHitCollider() { return hitCollider; }
USphereComponent *ABase_Enemy::GetDetectionSphere() { return DetectionRadius; }

// Called when the game starts or when spawned
void ABase_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	Stats->OnDamageTaken.AddDynamic(this, &ABase_Enemy::OnDamageTaken);
	hitCollider->OnComponentBeginOverlap.AddDynamic(this, &ABase_Enemy::OnActorOverlap);

	MovementComp->MaxWalkSpeed = Stats->GetSpeed();
}

// Called every frame
void ABase_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABase_Enemy::OnDamageTaken(UStats_Component* OwningStatsComp, float Health, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.f)
	{
		// Die!
		UE_LOG(LogTemp, Warning, TEXT("Die!"));
	}
}

void ABase_Enemy::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Cast<ABase_AIController>(GetController())->OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
