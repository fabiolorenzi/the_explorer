#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Teletransport.generated.h"

UENUM(BlueprintType)
enum class ETeletransportType: uint8 {
	Level_One,
	Level_Two
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTeletransportCatchEvent, ETeletransportType, TeletransportType);

UCLASS()
class THEEXPLORER_API ATeletransport : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeletransport();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Catch();

	void RotateCrystal(float rotationValue);

	UPROPERTY(EditAnywhere)
		ETeletransportType TeletransportType = ETeletransportType::Level_One;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FTeletransportCatchEvent TeletransportCatchEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TeletransportVars)
		float RotationValue;

	UPROPERTY(EditAnywhere)
		FName LevelName;
};