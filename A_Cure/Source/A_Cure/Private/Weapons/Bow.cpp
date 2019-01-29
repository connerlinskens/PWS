// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Weapons/Bow.h"
#include "Engine/World.h"
#include "Public/Weapons/Arrow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Public/Base_Class_Character.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

ABow::ABow()
{
	ArrowSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Arrow Spawn Point"));
	ArrowSpawnPoint->SetupAttachment(RootComponent);

	weaponType = WeaponType::Ranged;
	ArrowSocketName = FName("ArrowSocket");

	// Defaults
	LineTraceRange = 1000.f;
	tensioningBow = false;
	ArrowSpeed = 0.f;
	MaxArrowSpeed = 5000.f;
	ArrowTractionSpeed = 1.2f;
}

void ABow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (tensioningBow)
	{
		ArrowSpeed += DeltaTime * MaxArrowSpeed * ArrowTractionSpeed;
		if (ArrowSpeed >= 4500)
		{
			ArrowSpeed = 4500;
		}
		UE_LOG(LogTemp, Warning, TEXT("ArrowSpeed: %f"), ArrowSpeed)
	}
}

void ABow::Attack()
{
	tensioningBow = true;

	FVector EyeLocation;
	FRotator EyeRotation;
	myOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector ShootDirection = EyeRotation.Vector();

	FVector TraceEnd = EyeLocation + (ShootDirection * LineTraceRange);

	FHitResult hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(myOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;

	GetWorld()->LineTraceSingleByChannel(hit, EyeLocation, TraceEnd, ECollisionChannel::ECC_Pawn, QueryParams);
	
	FRotator ArrowRot;

	if (hit.GetActor())
	{
		FRotator ArrowRot = UKismetMathLibrary::FindLookAtRotation(EyeLocation, hit.ImpactPoint);
	}

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SpawnedArrow = Cast<AArrow>(GetWorld()->SpawnActor<AActor>(ArrowClass, mesh->GetSocketLocation(ArrowSocketName), mesh->GetSocketRotation(ArrowSocketName), spawnParams));

	SpawnedArrow->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	SpawnedArrow->SetActorRotation(ArrowRot);

	SpawnedArrow->SetOwner(myOwner);
	
}

void ABow::ReleaseArrow()
{
	SpawnedArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SpawnedArrow->Launch(ArrowSpeed);
	tensioningBow = false;
	ArrowSpeed = 0.f;
}