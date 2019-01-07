// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Base_Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Public/Stats_Component.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABase_Character::ABase_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setting different capsule size
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Disabling character rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Enabeling rotationToMovement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	// Creating CameraArm
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 300.0f;
	CameraArm->bUsePawnControlRotation = true;

	// Creating Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);
	Camera->bUsePawnControlRotation = false;

	StatsComponent = CreateDefaultSubobject<UStats_Component>(TEXT("Stats"));

	// Defaults
	bIsInLockMode = false;
}

// Called to bind functionality to input
void ABase_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	// Movement of character binding
	PlayerInputComponent->BindAxis("MoveForward", this, &ABase_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABase_Character::MoveRight);

	// Camera movement binding
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("DamageTest", IE_Pressed, this, &ABase_Character::Damage);
}

// Getters
UStats_Component *ABase_Character::GetStats() { return StatsComponent; }

// Called when the game starts or when spawned
void ABase_Character::BeginPlay()
{
	Super::BeginPlay();
	
	StatsComponent->OnDamageTaken.AddDynamic(this, &ABase_Character::OnDamageTaken);
}

void ABase_Character::MoveForward(float value)
{
	if (value != 0.0f && !bIsInLockMode)
	{
		// Finding out what forward is
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Turning into forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Adding the movement
		AddMovementInput(Direction, value);
	}
}

void ABase_Character::MoveRight(float value)
{
	if (value != 0.0f && !bIsInLockMode)
	{
		// Finding out what forward is
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Turning into forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Adding the movement
		AddMovementInput(Direction, value);
	}
}

// Called every frame
void ABase_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABase_Character::Damage()
{
	UGameplayStatics::ApplyDamage(this, StatsComponent->GetAttackDamage(), GetInstigatorController(), this, NormalDamageType);
}

void ABase_Character::OnDamageTaken(UStats_Component* OwningStatsComp, float Health, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.f)
	{
		// Die!
		UE_LOG(LogTemp, Warning, TEXT("This man is no longer living on this planet."));
	}
}
