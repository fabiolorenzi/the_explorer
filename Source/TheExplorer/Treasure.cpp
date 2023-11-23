#include "Treasure.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ATreasure::ATreasure()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATreasure::BeginPlay()
{
	Super::BeginPlay();
}

void ATreasure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateCrystal(RotationValue);
}

void ATreasure::Catch()
{
	UGameplayStatics::PlaySound2D(this, CatchSound);
	Destroy();
}

void ATreasure::RotateCrystal(float rotationValue)
{
	AddActorLocalRotation(FRotator(0, GetActorRotation().Vector().Z + rotationValue, 0));
}