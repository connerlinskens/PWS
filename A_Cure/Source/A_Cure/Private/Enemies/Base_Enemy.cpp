// Fill out your copyright notice in the Description page of Project Settings.

#include "public/Enemies/Base_Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/Stats_Component.h"

// Sets default values
ABase_Enemy::ABase_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setting the defaults values of the variables
	collider = GetCapsuleComponent();
	MovementComp = GetCharacterMovement();
	MovementSpeed = 250.0f;
	AttackRange = 150.f;
	AttackSpeed = 2.0f;

	// Setting op the hitbox
	hitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Box"));
	hitCollider->SetupAttachment(RootComponent);

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range"));
	AttackSphere->SetupAttachment(RootComponent);
	AttackSphere->SetSphereRadius(AttackRange);

	Stats = CreateDefaultSubobject<UStats_Component>(TEXT("Stats Component"));

	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack box"));
	AttackBox->SetupAttachment(RootComponent);
	AttackBox->InitBoxExtent(FVector(70.f));
	AttackBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Let the pawn get auto possessed by an AI controller
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Getters
USphereComponent *ABase_Enemy::GetAttackSphere() { return AttackSphere; }
float ABase_Enemy::GetAttackSpeed() { return AttackSpeed; }
UBoxComponent *ABase_Enemy::GetAttackBox() { return AttackBox; }
UStats_Component *ABase_Enemy::GetStats() { return Stats; }
TSubclassOf<UDamageType> ABase_Enemy::GetDamageType() { return NormalDamageType; }

// Called when the game starts or when spawned
void ABase_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	Stats->OnDamageTaken.AddDynamic(this, &ABase_Enemy::OnDamageTaken);

	MovementComp->MaxWalkSpeed = MovementSpeed;
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
