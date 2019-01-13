// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Class_Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/Stats_Component.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Public/Weapons/Base_Weapon.h"

// Sets default values
ABase_Class_Character::ABase_Class_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(35.f, 96.f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FP Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Arms->SetupAttachment(Camera);

	Stats = CreateDefaultSubobject<UStats_Component>(TEXT("Stats"));

	// Defaults
	DashSpeed = 200.f;
	Dashed = false;
	DashDelay = 3.f;
	Zoom = false;
	ZoomSpeed = 3.f;
}

// Called to bind functionality to input
void ABase_Class_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABase_Class_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABase_Class_Character::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ABase_Class_Character::Dash);

	PlayerInputComponent->BindAction("DamageTest", IE_Pressed, this, &ABase_Class_Character::Damage);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ABase_Class_Character::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ABase_Class_Character::ZoomOut);
}

// Getters ans Setters
UStats_Component *ABase_Class_Character::GetStats() { return Stats; }

// Called when the game starts or when spawned
void ABase_Class_Character::BeginPlay()
{
	Super::BeginPlay();

	Stats->OnDamageTaken.AddDynamic(this, &ABase_Class_Character::OnDamageTaken);

	if (ClassWeapon != nullptr)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FAttachmentTransformRules attachRules(EAttachmentRule::SnapToTarget, false);
		Weapon = GetWorld()->SpawnActor<ABase_Weapon>(ClassWeapon, spawnParams);
		Weapon->AttachToComponent(Arms, attachRules, WeaponSocketName);
		
	}

	NormalPOV = Camera->FieldOfView;
}

// Called every frame
void ABase_Class_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Zoom)
	{
		Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, ZoomedPOV, DeltaTime, ZoomSpeed));
	}
	else
	{
		Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, NormalPOV, DeltaTime, ZoomSpeed));
	}
}


void ABase_Class_Character::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

void ABase_Class_Character::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector(), value);
}

void ABase_Class_Character::Dash()
{
	if (!Dashed)
	{
		FVector LaunchVelocity = GetVelocity();
		LaunchVelocity.Z = 0;
		LaunchVelocity.Normalize();
		LaunchVelocity *= DashSpeed;

		LaunchCharacter(LaunchVelocity, false, false);

		Dashed = true;
		GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &ABase_Class_Character::SetDashFalse, DashDelay, false);
	}
}

void ABase_Class_Character::Damage()
{
	UGameplayStatics::ApplyDamage(this, Stats->GetAttackDamage(), GetInstigatorController(), this, NormalDamageType);
}

void ABase_Class_Character::OnDamageTaken(UStats_Component* OwningStatsComp, float Health, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.f)
	{
		// Die!
		UE_LOG(LogTemp, Warning, TEXT("This man is no longer living on this planet."));
	}
}

void ABase_Class_Character::SetDashFalse()
{
	Dashed = false;
}

void ABase_Class_Character::ZoomIn()
{
	Zoom = true;
}

void ABase_Class_Character::ZoomOut()
{
	Zoom = false;
}