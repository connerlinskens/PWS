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
#include "Kismet/KismetMathLibrary.h"

ABase_AIController::ABase_AIController()
{
	// Defaults
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

	ownCharacter->GetDetectionSphere()->GetOverlappingActors(OverlappingActors, CharacterClass);

	for (int x = 0; x < OverlappingActors.Num(); x++)
	{
		if (Cast<ABase_Class_Character>(OverlappingActors[x]))
		{
			MoveToPlayer();
		}
		else
		{
			StopMovement();
		}
	}

}

void ABase_AIController::MoveToPlayer()
{
	MoveToActor(player, 1.f);
}

void ABase_AIController::AttackPlayer()
{
	StopMovement();

	// Getting the rotation between the enemy and the player
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(ownCharacter->GetActorLocation(), player->GetActorLocation());

	// Turning The rotation into vector and add the knockback variable
	FVector Direction = rot.Vector();
	Direction.Normalize();
	Direction *= ownCharacter->GetStats()->GetKnockBack();

	// Doing damage and giving knockback
	UGameplayStatics::ApplyDamage(player, ownCharacter->GetStats()->GetAttackDamage(), GetInstigatorController(), this, ownCharacter->GetDamageType());
	player->LaunchCharacter(Direction, true, true);

}

void ABase_AIController::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == player)
	{
		AttackPlayer();
	}
}