// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Ranger_Class.h"
#include "Components/InputComponent.h"
#include "Public/Weapons/Bow.h"

ARanger_Class::ARanger_Class()
{
	// Defaults
	WeaponSocketName = FName("BowSocket");
}

void ARanger_Class::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("UseWeapon", IE_Released, this, &ARanger_Class::ReleaseArrow);
}

void ARanger_Class::BeginPlay()
{
	Super::BeginPlay();

	myBow = Cast<ABow>(Weapon);
}

void ARanger_Class::ReleaseArrow()
{
	if (myBow)
	{
		myBow->ReleaseArrow();
		ZoomOut();
	}
}