// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Class_Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABase_Class_Character::ABase_Class_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(35.f, 96.f);

	GetMesh()->Rename(TEXT("Arms"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FP Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	// Defaults
	DashSpeed = 200.f;
}

// Called when the game starts or when spawned
void ABase_Class_Character::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABase_Class_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	UE_LOG(LogTemp, Warning, TEXT("Dash!"));

	FVector LaunchVelocity = GetVelocity();
	LaunchVelocity.Z = 0;
	LaunchVelocity *= DashSpeed;

	LaunchCharacter(LaunchVelocity, false, false);
}

