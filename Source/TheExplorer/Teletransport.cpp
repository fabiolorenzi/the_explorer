#include "Teletransport.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ATeletransport::ATeletransport()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATeletransport::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATeletransport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateCrystal(RotationValue);
}

void ATeletransport::Catch()
{
	TeletransportCatchEvent.Broadcast(TeletransportType);
}

void ATeletransport::RotateCrystal(float rotationValue)
{
	AddActorLocalRotation(FRotator(0, GetActorRotation().Vector().Z + rotationValue, 0));
}