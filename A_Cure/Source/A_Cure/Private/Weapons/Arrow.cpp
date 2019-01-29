// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Weapons/Arrow.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Public/Enemies/Base_Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Ranger_Class.h"
#include "Public/Stats_Component.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AArrow::AArrow()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ArrowBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow base"));
	ArrowBase->SetupAttachment(RootComponent);
	//RootComponent = ArrowBase;
	ArrowBase->SetEnableGravity(false);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	MovementComp->bAutoActivate = false;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	HitBox->SetupAttachment(RootComponent);
	HitBox->SetEnableGravity(false);
	HitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AArrow::FreezeArrow);

	GravityStrength = 0.5f;

	MovementComp->ProjectileGravityScale = GravityStrength;
}

// Setters
void AArrow::SetOwner(ABase_Class_Character *Owner)
{
	myOwner = Owner;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}


void AArrow::Launch(float Speed)
{
	ArrowBase->SetEnableGravity(true);
	HitBox->SetEnableGravity(true);

	MovementComp->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	MovementComp->Activate();
}

void AArrow::FreezeArrow(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	bool environment = false;
	ABase_Enemy *CurrentEnemy = nullptr;

	if (OtherActor->ActorHasTag(FName("Environment")))
	{
		environment = true;
	}
	else if (Cast<ABase_Enemy>(OtherActor))
	{
		CurrentEnemy = Cast<ABase_Enemy>(OtherActor);
	}

	if(CurrentEnemy)
	{
		if (OtherComp == CurrentEnemy->GetHitCollider())
		{
			MovementComp->StopMovementImmediately();
			MovementComp->Deactivate();

			this->AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
			ArrowBase->SetEnableGravity(false);
			HitBox->SetEnableGravity(false);

			CurrentEnemy->ChildActors.Add(this);

			UGameplayStatics::ApplyDamage(CurrentEnemy, myOwner->GetStats()->GetAttackDamage(), GetInstigatorController(), myOwner, myOwner->GetStats()->GetDamageType());

			FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentEnemy->GetActorLocation());

			FVector Direction = rot.Vector();
			Direction.Normalize();
			Direction.Z = 1;
			Direction *= myOwner->GetStats()->GetKnockBack();

			CurrentEnemy->SetStunned(true);
			CurrentEnemy->LaunchCharacter(Direction, true, true);
		}
	}
	else if (environment)
	{
		environment = false;

		MovementComp->StopMovementImmediately();
		MovementComp->Deactivate();

		this->AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
		ArrowBase->SetEnableGravity(false);
		HitBox->SetEnableGravity(false);

		GetWorld()->GetTimerManager().SetTimer(destructTimer, this, &AArrow::DestroyArrow, 5.f, false);
	}
}

void AArrow::DestroyArrow()
{
	Destroy();
}