// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SkateCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include <UI/W_SkateHUD.h>

// Sets default values
ASkateCharacter::ASkateCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Remove after importing the character
    GetMesh()->DestroyComponent();      //nvm it will probably make bugs

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMesh->SetupAttachment(RootComponent);
    SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    SkateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateMesh"));
    SkateMesh->SetupAttachment(SkeletalMesh);
    SkateMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(SkateMesh);
    FollowCamera->SetRelativeLocation(FVector(-100.f, 0, 150.f));

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    GetCharacterMovement()->JumpZVelocity = JumpHeight;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 200.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 200.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 300.f;
    GetCharacterMovement()->AirControl = 0.3f;
    GetCharacterMovement()->AirControlBoostMultiplier = .1f;
    GetCharacterMovement()->AirControlBoostVelocityThreshold = 100.f;


}



// Called when the game starts or when spawned
void ASkateCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (HUDClass && IsPlayerControlled())
    {
        CreateWidget<UUserWidget>(GetWorld(), HUDClass)->AddToViewport();
    }
}

// Called every frame
void ASkateCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASkateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}


    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInput->BindAction(BoostAction, ETriggerEvent::Started, this, &ASkateCharacter::OnBoostStarted);
        EnhancedInput->BindAction(BoostAction, ETriggerEvent::Completed, this, &ASkateCharacter::OnBoostEnded);
        EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Started, this, &ASkateCharacter::OnMoveForward);
        EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Completed, this, &ASkateCharacter::OnStopMovingForward);
        EnhancedInput->BindAction(BreakAction, ETriggerEvent::Started, this, &ASkateCharacter::OnStartBreaking);
        EnhancedInput->BindAction(BreakAction, ETriggerEvent::Completed, this, &ASkateCharacter::OnStopBreaking);
        EnhancedInput->BindAction(TurnLeftAction, ETriggerEvent::Triggered, this, &ASkateCharacter::OnTurnLeft);
        EnhancedInput->BindAction(TurnRightAction, ETriggerEvent::Triggered, this, &ASkateCharacter::OnTurnRight);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ASkateCharacter::OnJump);

    }

}

void ASkateCharacter::OnBoostStarted()
{
    if (!bCanBoost || !GetCharacterMovement()->IsMovingOnGround()) return;

    CurrentBoostHoldTime = 0.f;
    GetWorld()->GetTimerManager().SetTimer(BoostHoldTimer, [this]()
        {
            CurrentBoostHoldTime += 0.1f;
        }, 0.1f, true);
    
}

void ASkateCharacter::OnBoostEnded()
{
    if (!bCanBoost || !GetCharacterMovement()->IsMovingOnGround() || CurrentBoostHoldTime <= MinBoostDuration)
        return;

    GetWorld()->GetTimerManager().ClearTimer(BoostHoldTimer);

    // Calculate boost strength
    const float BoostStrength = FMath::Clamp(CurrentBoostHoldTime, MinBoostMolitplier, MaxBoostDuration);
    const FVector BoostImpulse = GetActorForwardVector() * BoostStrength * BoostPower;

    // Get current velocity (horizontal only)
    const FVector CurrentVelocity = GetVelocity();
    const float CurrentSpeed = FVector(CurrentVelocity.X, CurrentVelocity.Y, 0).Size();

    // Define speed limits
    const float SpeedAfterBoost = CurrentSpeed + (BoostImpulse.Size() / GetCharacterMovement()->Mass);

    // Apply reduced boost if over threshold
    if (SpeedAfterBoost > MaxAllowedSpeed)
    {
        const float ReductionFactor = (MaxAllowedSpeed - CurrentSpeed) / (BoostImpulse.Size() / GetCharacterMovement()->Mass);
        const FVector ClampedImpulse = BoostImpulse * FMath::Max(0, ReductionFactor);

        GetCharacterMovement()->AddImpulse(ClampedImpulse, true);
        
    }
    else
    {
        GetCharacterMovement()->AddImpulse(BoostImpulse, true);
    }

    bCanBoost = false;
    OnBoostCooldownChanged.Broadcast(true);

    GetWorld()->GetTimerManager().SetTimer(
        BoostCooldownTimer,
        [this]() { bCanBoost = true; OnBoostCooldownChanged.Broadcast(false); },
        BoostCooldown,
        false
    );
}


void ASkateCharacter::OnMoveForward()
{
    isMovingForward = true;
}

void ASkateCharacter::OnStopMovingForward()
{
    isMovingForward = false;
}

void ASkateCharacter::OnStartBreaking()
{
    if (bIsBreaking || !GetCharacterMovement()->IsMovingOnGround())
        return;

    bIsBreaking = true;

    // Calculate breaking duration based on current speed
    const float CurrentSpeed = GetVelocity().Size2D();
    const float SpeedRatio = FMath::Clamp(CurrentSpeed / GetCharacterMovement()->MaxWalkSpeed, 0.1f, 1.0f);
    const float ActualBreakingTime = MaxBreakingLength * SpeedRatio;

    // Apply immediate friction
    GetCharacterMovement()->BrakingDecelerationWalking += BreakingFriction * 1000.f;

    // Set up gradual breaking
    GetWorld()->GetTimerManager().SetTimer(
        BreakingTimer,
        this,
        &ASkateCharacter::ApplyBreakingForce,
        0.016f, // ~60fps update
        true
    );

}

void ASkateCharacter::OnStopBreaking()
{
    if (!bIsBreaking) return;

    GetWorld()->GetTimerManager().ClearTimer(BreakingTimer);
    GetCharacterMovement()->BrakingDecelerationWalking -= BreakingFriction * 1000.f;
    bIsBreaking = false;
}

void ASkateCharacter::ApplyBreakingForce()
{
    if (!bIsBreaking) return;

    const FVector CurrentVelocity = GetVelocity();
    const float CurrentSpeed = CurrentVelocity.Size2D();

    if (CurrentSpeed < 10.f) // Nearly stopped
    {
        OnStopBreaking();
        return;
    }

    // Calculate new velocity (linear reduction)
    const float SpeedReduction = (GetCharacterMovement()->MaxWalkSpeed / MaxBreakingLength) * 0.016f;
    const FVector NewVelocity = CurrentVelocity.GetSafeNormal() * FMath::Max(0, CurrentSpeed - SpeedReduction);

    GetCharacterMovement()->Velocity = NewVelocity;
}

void ASkateCharacter::OnTurnLeft()
{
    const float TurnInput = -1.f;
    HandleTurning(TurnInput);
}

void ASkateCharacter::OnTurnRight()
{
    const float TurnInput = 1.f;
    HandleTurning(TurnInput);
}

void ASkateCharacter::HandleTurning(float TurnInput)
{

    {
        const FVector AdjustedVelocity = GetActorForwardVector() * GetVelocity().Size2D();
        GetCharacterMovement()->Velocity = FVector(
            AdjustedVelocity.X,
            AdjustedVelocity.Y,
            GetCharacterMovement()->Velocity.Z // Preserve Z
        );
        AddControllerYawInput(TurnInput * TurnRate * GetWorld()->GetDeltaSeconds());
    }

}


void ASkateCharacter::OnJump()
{
    if (GetCharacterMovement()->IsMovingOnGround())
    {
        const float ImpulseZ = FMath::Sqrt(JumpHeight * -2.f * GetCharacterMovement()->GetGravityZ());
        GetCharacterMovement()->AddImpulse(FVector(0, 0, ImpulseZ), true);
    }
}





