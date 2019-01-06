// Fill out your copyright notice in the Description page of Project Settings.

#include "public/Enemies/Base_Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABase_Enemy::ABase_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setting op the hitbox
	hitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Box"));

	// Let the pawn get auto possessed by an AI controller
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Setting the defaults values of the variables
	collider = GetCapsuleComponent();
	MovementComp = GetCharacterMovement();
	MovementSpeed = 250.0f;
}

// Called when the game starts or when spawned
void ABase_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	MovementComp->MaxWalkSpeed = MovementSpeed;
}

// Called every frame
void ABase_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABase_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

