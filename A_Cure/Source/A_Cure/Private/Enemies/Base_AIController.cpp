// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Enemies/Base_AIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Public/Enemies/Base_Enemy.h"
#include "Components/SphereComponent.h"
#include "Public/Base_Class_Character.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Public/Stats_Component.h"

ABase_AIController::ABase_AIController()
{
	player = nullptr;
	Attacked = false;
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
	if (!Attacked)
	{
		ownCharacter->GetAttackBox()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		ownCharacter->GetAttackBox()->GetOverlappingActors(AttackingActors, CharacterClass);

		for (int x = 0; x < AttackingActors.Num(); x++)
		{
			if (Cast<ABase_Class_Character>(AttackingActors[x]))
			{
				Attacked = true;
				UGameplayStatics::ApplyDamage(player, ownCharacter->GetStats()->GetAttackDamage(), GetInstigatorController(), this, ownCharacter->GetDamageType());
				GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABase_AIController::ResetAttack, ownCharacter->GetAttackSpeed(), false);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Attack!"));
	}
}

void ABase_AIController::ResetAttack()
{
	Attacked = false;
}