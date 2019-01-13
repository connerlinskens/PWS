// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Weapons/Base_Weapon.h"
#include "Components/SkeletalMeshComponent.h"

ABase_Weapon::ABase_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = Root;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bow"));
	mesh->SetupAttachment(RootComponent);
	mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
//  Setting default values
	weaponType = WeaponType::None;
}

// Called when the game starts or when spawned
void ABase_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABase_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

