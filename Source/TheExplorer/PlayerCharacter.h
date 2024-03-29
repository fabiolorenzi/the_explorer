#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "PlayerCharacter.generated.h"

class USoundCue;

UCLASS()
class THEEXPLORER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerVars)
		bool IsInPause;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerVars)
		bool IsJumping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerVars)
		bool HasAttacked;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerVars)
		float Life;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerVars)
		int LevelTreasures;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerVars)
		bool IsHubLevel;

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		void RestartGame();
	UFUNCTION(BlueprintCallable)
		void ReturnToHub();

	bool IsPlayerDead;

	void Jump();
	void Attack();
	void ResetAttack();
	
	void StopJumping();
	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void Death();
	void PauseGame();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		USoundCue* DrownSound;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};