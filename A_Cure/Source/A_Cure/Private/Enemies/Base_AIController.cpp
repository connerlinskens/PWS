// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Enemies/Base_AIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Public/Enemies/Base_Enemy.h"
#include "Components/SphereComponent.h"
#include "Public/Base_Class_Character.h"

ABase_AIController::ABase_AIController()
{
	player = nullptr;
}

void ABase_AIController::BeginPlay()
{
	Super::BeginPlay();

	ownCharacter = Cast<ABase_Enemy>(GetCharacter());
	player = GetWorld()->GetFirstPlayerController()->GetCharacter();
}

void ABase_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// first we move to the player
	MoveToPlayer();

	// Get all overlaping actors in the attach sphere
	ownCharacter->GetAttackSphere()->GetOverlappingActors(OverlappingActors, CharacterClass);

	for (int x = 0; x < OverlappingActors.Num(); x++)
	{
		// Check if one of the overlapping actors is the player
		if (Cast<ABase_Class_Character>(OverlappingActors[x]))
		{
			StopMovement();
			
			AttackPlayer();
		}
	}
}

void ABase_AIController::MoveToPlayer()
{
	MoveToActor(player, 1.f);
}

void ABase_AIController::AttackPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack!"));
}