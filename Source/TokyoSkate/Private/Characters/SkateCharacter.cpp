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

// Sets default values
ASkateCharacter::ASkateCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Remove after importing the character
    GetMesh()->DestroyComponent();

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateMesh"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(MeshComponent);
    FollowCamera->SetRelativeLocation(FVector(-100.f, 0, 150.f));

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    GetCharacterMovement()->JumpZVelocity = JumpHeight;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 200.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 200.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 300.f;

}

void ASkateCharacter::TestRawInput()
{
    UE_LOG(LogTemp, Warning, TEXT("ENHANCED INPUT WORKING"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("INPUT CONFIRMED"));
}

// Called when the game starts or when spawned
void ASkateCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (!GetController())
    {
        UE_LOG(LogTemp, Error, TEXT("No controller possessing this character!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Controller: %s"), *GetController()->GetName());
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

    UE_LOG(LogTemp, Warning, TEXT("Input Component Class: %s"), *PlayerInputComponent->GetClass()->GetName());

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

        EnhancedInput->BindAction(TestAction, ETriggerEvent::Triggered, this, &ASkateCharacter::TestRawInput);

        UE_LOG(LogTemp, Warning, TEXT("Enhanced Input bindings initialized!"));
    }

}

void ASkateCharacter::OnBoostStarted()
{
    if (!bCanBoost || !GetCharacterMovement()->IsMovingOnGround()) return;

    CurrentBoostHoldTime = 0.f;
    GetWorld()->GetTimerManager().SetTimer(BoostHoldTimer, [this]()
        {
            CurrentBoostHoldTime += 0.1f; // Update every 0.1 seconds
        }, 0.1f, true);
    
}

void ASkateCharacter::OnBoostEnded()
{
    if (!bCanBoost) return;

    GetWorld()->GetTimerManager().ClearTimer(BoostHoldTimer);

    float BoostStrength = FMath::Clamp(CurrentBoostHoldTime, MinBoostDuration, MaxBoostDuration);

    // Apply single impulse
    GetCharacterMovement()->AddImpulse(
        GetActorForwardVector() * BoostStrength * BoostPower,
        true
    );

    bCanBoost = false;
    GetWorld()->GetTimerManager().SetTimer(
        BoostCooldownTimer,
        [this]() { bCanBoost = true; },
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
    isBreaking = true;
}

void ASkateCharacter::OnStopBreaking()
{
    isBreaking = false;
}

void ASkateCharacter::OnTurnLeft()
{
    AddControllerYawInput(-1.f * TurnRate * GetWorld()->GetDeltaSeconds());

    const FVector CurrentVelocity = GetVelocity();
    const float CurrentSpeed = CurrentVelocity.Size();

    const FRotator CurrentRot = GetActorRotation();
    const FRotator TargetRot = CurrentRot + FRotator(0, -1 * TurnRate * GetWorld()->GetDeltaSeconds(), 0);

    // Redirect velocity while preserving magnitude
    const FVector NewDirection = TargetRot.Vector();
    const FVector NewVelocity = NewDirection * CurrentSpeed;

    GetCharacterMovement()->Velocity = NewVelocity;

    SetActorRotation(TargetRot);
}

void ASkateCharacter::OnTurnRight()
{
    AddControllerYawInput(1.f * TurnRate * GetWorld()->GetDeltaSeconds());

    const FVector CurrentVelocity = GetVelocity();
    const float CurrentSpeed = CurrentVelocity.Size();

    const FRotator CurrentRot = GetActorRotation();
    const FRotator TargetRot = CurrentRot + FRotator(0, 1 * TurnRate * GetWorld()->GetDeltaSeconds(), 0);

    // Redirect velocity while preserving magnitude
    const FVector NewDirection = TargetRot.Vector();
    const FVector NewVelocity = NewDirection * CurrentSpeed;

    GetCharacterMovement()->Velocity = NewVelocity;

    SetActorRotation(TargetRot);
}


void ASkateCharacter::OnJump()
{
    if (GetCharacterMovement()->IsMovingOnGround())
    {
        const float ImpulseZ = FMath::Sqrt(JumpHeight * -2.f * GetCharacterMovement()->GetGravityZ());
        GetCharacterMovement()->AddImpulse(FVector(0, 0, ImpulseZ), true);
    }
}





