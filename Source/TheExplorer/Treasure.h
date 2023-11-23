// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Treasure.generated.h"

class USoundCue;

UCLASS()
class THEEXPLORER_API ATreasure : public APawn
{
	GENERATED_BODY()

public:
	ATreasure();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Catch();

	void RotateCrystal(float rotationValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TreasureVars)
		float RotationValue;

private:
	UPROPERTY(EditAnywhere)
		USoundCue* CatchSound;
};