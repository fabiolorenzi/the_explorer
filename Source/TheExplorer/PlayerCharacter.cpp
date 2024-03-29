#include "PlayerCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "Teletransport.h"
#include "Treasure.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 450.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	IsInPause = false;
	IsPlayerDead = false;
	IsJumping = false;
	HasAttacked = false;
	Life = 100.0f;
	LevelTreasures = 0;
}

void APlayerCharacter::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("DeathLine")) {
		UGameplayStatics::PlaySound2D(this, DrownSound);
		Death();
	} else if (OtherActor->ActorHasTag("Teletransport_Level_One") || OtherActor->ActorHasTag("Teletransport_Level_Two")) {
		UE_LOG(LogTemp, Warning, TEXT("Touched"));
		Cast<ATeletransport>(OtherActor)->Catch();
	} else if (OtherActor->ActorHasTag("Treasure") && !IsHubLevel) {
		LevelTreasures += 1;
		Life = 100.0f;
		Cast<ATreasure>(OtherActor)->Catch();
	};
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBeginOverlap);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Life <= 0) {
		if (!IsPlayerDead) {
			IsPlayerDead = true;
			GetMesh()->SetSimulatePhysics(true);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::RestartGame, 3.0f, false);
		};
	} else if (!IsHubLevel) {
		Life -= DeltaTime;
	};
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCharacter::PauseGame).bExecuteWhenPaused = true;

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

void APlayerCharacter::Jump()
{
	ACharacter::Jump();
	IsJumping = true;
}

void APlayerCharacter::Attack()
{
	if (!HasAttacked && !IsJumping) {
		HasAttacked = true;
		UE_LOG(LogTemp, Warning, TEXT("Attack"));

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::ResetAttack, 0.5f, false);
	};
}

void APlayerCharacter::ResetAttack()
{
	HasAttacked = false;
}

void APlayerCharacter::StopJumping()
{
	ACharacter::StopJumping();
	IsJumping = false;
}

void APlayerCharacter::MoveForward(float Axis)
{
	if (!IsPlayerDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	};
}

void APlayerCharacter::MoveRight(float Axis)
{
	if (!IsPlayerDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	};
}

void APlayerCharacter::Death()
{
	IsPlayerDead = true;
	Life = 0.0f;
	GetMesh()->SetSimulatePhysics(true);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::RestartGame, 5.0f, false);
}

void APlayerCharacter::PauseGame()
{
	IsInPause = !IsInPause;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Pause();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = IsInPause;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bEnableClickEvents = IsInPause;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bEnableMouseOverEvents = IsInPause;
}

void APlayerCharacter::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void APlayerCharacter::ReturnToHub()
{
	UGameplayStatics::OpenLevel(this, FName("HubLevel"), false);
}