// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Enemies/Base_AIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Public/Base_Character.h"

ABase_AIController::ABase_AIController()
{
	player = nullptr;
}

void ABase_AIController::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<ABase_Character>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}

void ABase_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveToPlayer();
}

void ABase_AIController::MoveToPlayer()
{
	MoveToActor(player, 1.f);
}