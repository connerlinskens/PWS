// Copyright Conner Linskens 2018-2019


/*
In C++ wordt gebruik gemaakt van header en cpp files.
In de header files worden variablen, classes, enums en functies gedeclareerd.
In de cpp files staat het interesanten gedeelte want hier wordt een betekenis
gegeven aan al die dingen.
Dit is een cpp file.
*/

#include "Public/Weapons/Bow.h"
#include "Engine/World.h"
#include "Public/Weapons/Arrow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Public/Base_Class_Character.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Public/Stats_Component.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

/*
In de taal C++ wordt bij het begin van een file de "includes" geplaats,
zoals hierboven. Deze worden uitgevoerd voordat de compiler zijn werk
doet. De functie van de include is om andere files te importeren in deze
file. Deze mogelijkheid zorgt voor het gebruik van classes en functies
die gemaakt zijn in andere files. Dit is handig zodat je je classes
allemaal in aparte files kan hebben en niet dat je eindigt met een
hele lange file. Dit zorgt uiteindelijk voor duidelijkheid.
*/

ABow::ABow()
{
	ArrowSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Arrow Spawn Point"));
	ArrowSpawnPoint->SetupAttachment(RootComponent);

	weaponType = WeaponType::Ranged;

	// Default variable values
	LineTraceRange = 1000.f;
	tensioningBow = false;
	ArrowSpeed = 0.f;
	MaxArrowSpeed = 5000.f;
	ArrowTractionSpeed = 1.2f;
	bCanFire = true;
	FireShakeDelay = 1.5f;
	TensoningBowSpeed = 150.f;
}

/*
Hierboven is een "constructor" te zien. Elke class is bezit van
minimaal één constructor. Deze functie wordt geroepen wanneer
een instantie van deze class gemaakt wordt. In de constructor
kan je allerlei dingen plaatsen. Meestal geef je benodigde 
variablen een waarde, dat gebeurt hierboven ook. Dit is super
handig want dan kan je als het waren je instantie van de class
klaar zetten voor gebruik.
In deze constructor wordt er een waarde aangegeven aan de
ArrowSpawnPoint variabel. Deze variabel is gedeclareerd in de header
file van deze class. Er wordt doormiddel van een ingebouwde functie
van de Unreal Engine een waarde toegegeven. Deze functie
maakt een object aan genaamd USceneComponent. Dit is een class
die een locatie weergeeft in het spel, ideaal voor het maken
van een spawn point voor in dit geval pijlen voor de boog.
We maken de spawnpoint vast aan de "rootcomponent" dit
is de "wortel" als het ware van het object. In dit geval
is dat de mesh (het 3D model).
Hierna wordt er een weaponType gegeven aan deze boog in
dit geval is dat natuurlijk ranged want we willen het spel laten
weten dat de speler in bezit is van een ranged wapen en dus
daar mee kan schieten en niet slaan.
Als laatst wordt er nog wat waardes gegeven aan verschillende
variabelen die we nodig hebben om de boog te laten werken. 
Deze variabelen bestaan dan ook uit allerlei dingen van 
span-snelheid tot schiet range
*/


void ABow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (tensioningBow)
	{
		// slowly increasing the arrowspeed
		ArrowSpeed += DeltaTime * MaxArrowSpeed * ArrowTractionSpeed;
		if (ArrowSpeed >= 4500)
		{
			ArrowSpeed = 4500;
		}
	}
}

/*
Hierboven is de Tick functie te zien. Deze functie wordt
elke frame van het spel geroepen. In deze functie kan je 
dus bijvoorbeeld waardes aanpassen naarmate van tijd. In
Dit geval wordt de snelheid van de pijl bepaald naarmate
hoelang je de rechtermuis knop inhoud. Doormiddel van een
if statement limiteren wij de snelheid tot een bepaald moment
zodat je niet met een oneindeige snelheid kan schieten.
*/

void ABow::Attack()
{
	if (bCanFire)
	{
		bCanFire = false;
		tensioningBow = true;

		DefaultMovementSpeed = myOwner->GetCharacterMovement()->MaxWalkSpeed;

		myOwner->GetCharacterMovement()->MaxWalkSpeed = TensoningBowSpeed;

		// Getting the eye location and rotation
		FVector EyeLocation;
		FRotator EyeRotation;
		myOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		// Making the end-point for our shoot destination
		FVector ShootDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShootDirection * LineTraceRange);

		// Setup for the line-trace
		FHitResult hit;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(myOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		// Doind the line-trace
		GetWorld()->LineTraceSingleByChannel(hit, EyeLocation, TraceEnd, ECollisionChannel::ECC_Pawn, QueryParams);
	
		// Making a arrow rotation variable
		FRotator ArrowRot;

		// If we hit an actor with the line-trace we want to get the rotation between the 2 objects
		if (hit.GetActor())
		{
			FRotator ArrowRot = UKismetMathLibrary::FindLookAtRotation(EyeLocation, hit.ImpactPoint);
		}

		// Setting up spawnparams so we can spawn a object
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawning the arrow
		SpawnedArrow = Cast<AArrow>(GetWorld()->SpawnActor<AActor>(ArrowClass, mesh->GetSocketLocation(ArrowSocketName), mesh->GetSocketRotation(ArrowSocketName), spawnParams));
		SpawnedArrow->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		SpawnedArrow->SetActorRotation(ArrowRot);
		SpawnedArrow->SetOwner(myOwner);

		// Resetting the attack
		GetWorld()->GetTimerManager().SetTimer(reloadTimerHandle, this, &ABow::Reload, myOwner->GetStats()->GetAttackRate(), false);
		GetWorld()->GetTimerManager().SetTimer(FireShakeTimerHandle, this, &ABow::FireShake, 0.01f, true, FireShakeDelay);
	}
}

/*
We moeten natuurlijk ook kunnen schieten dus daar hebben we
ook een functie voor. Deze functie wordt geroepen wanneer de
speler op de rechtermuis knop drukt, dit gebeurt in de weapon
class dus is nu niet te zien.
In deze functies gebeurt heel veel. Wanneer er geschoten wordt
checken we eerst ofdat de speler niet al geschoten heeft. Zodat
de speler niet achter elkaar kan schieten. Als de speler dus
nog niet geschoten heeft gaan we verder. Als eerst zetten we 
dus de boolean bCanShoot (een boolean kan true of false zijn) op
false, zodat de speler dus niet gelijk opnieuw kan schieten. We
zetten de boolean tensioningBow op true zodat de snelheid van de 
pijl toeneemt.
We slaan de snelheid van de player op en zetten de snelheid lager
zodat je slomer loopt wanneer je de boog spant.
Doormiddel van een Unreal Engine functies halen we de ooglocatie
en -rotatie op. Om een richting te krijgen van waar we de pijl
naar toe willen schieten maken we een vector van de oog rotatie.
We maken daarna een variabel aan die het eind punt van de schiet
locatie moet zijn. Deze maken wij door de richting en de range 
te vermenigvuldigen en deze op te tellen bij de ooglocatie.
We gaan een onzichtbare lijn afschieten om te bepalen waar we
precies naar toe gaan schieten, dit heet line-tracing. We maken
de benodigheden om een line-trace te maken. We willen dat de line-
trace de huidige actor negeerd. We kijken ofdat we iets raken.
Zo ja dan willen we een nieuwe rotatie maken voor onze pijl.
We maken SpawnParameters aan zodat we een pijl kunnen maken.
In deze parameters geven we aan: hoe dan ook spawn de pijl.
Hierna spawnen wij de pijl in de wereld op de juiste plek.
We maken de pijl aan de boog vast zodat deze pijl
meebeweegt met de speler en we geven de pijl haar nieuwe
rotatie.
Wanneer dit allemaal succesful is gegaan zorgen we ervoor
dat alles weer goed gereset wordt en de speler weer naar
een bepaalde tijd opnieuw kan schieten.
*/

void ABow::ReleaseArrow()
{
	SpawnedArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SpawnedArrow->Launch(ArrowSpeed);
	tensioningBow = false;
	ArrowSpeed = 0.f;
	myOwner->GetCharacterMovement()->MaxWalkSpeed = DefaultMovementSpeed;
}

/*
De functie ReleaseArrow wordt geroepen wanneer de speler
besluit de boog te laten schieten. Dit gebeurt doormiddel
van op de linkermuis knop te drukken.
Deze functies is eigenlijk best simpel. We koppelen de pijl
los van de boog. We schieten doormiddel van de Launch functie
de pijl af met de ArrowSpeed variabel. Daarna zetten we de
tensioningBow boolean weer terug op false, zodat de snelheid
niet meer groeit en zetten daarna de pijl snelheid weer terug
op 0.
Als laatst zetten we de snelheid van de speler weer terug 
op het oude.
*/

void ABow::Reload()
{
	bCanFire = true;
}

/*
Dit is de functie die dus de speler weer toegang geeft
tot het schieten van de boog
*/

void ABow::FireShake()
{
	if (myOwner && tensioningBow)
	{
		APlayerController *PC = Cast<APlayerController>(myOwner->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}
}

/*
De FireShake is een functie die om de 0.01 seconden
geroepen wordt nadat de speler de boog aan het spannen
is. We checken eerst ofdat de boog uberhaupt gespannen
is. Daarna halen we de controller op van de speler en 
checken ofdat dat gelukt is. Wanneer dit het geval is
spelen we een camera shake af. Dit gebeurt met een 
ingebouwde functie van Unreal Engine.
*/