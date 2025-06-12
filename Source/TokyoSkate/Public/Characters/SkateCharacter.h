// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "SkateCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBoostCooldownChanged, bool, bIsInCooldown);

UCLASS()
class TOKYOSKATE_API ASkateCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASkateCharacter();


	UPROPERTY(BlueprintAssignable, Category = "Animation")
	FOnBoostCooldownChanged OnBoostCooldownChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SkateMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FollowCamera;

	// Camera settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraFollowDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraHeightOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BoostAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BreakAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TurnLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TurnRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TestAction;



	// Boost params
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BoostCooldown = 1.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinBoostDuration = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxBoostDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BoostPower = 2400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxAllowedSpeed = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinBoostMolitplier = 0.6f;


	//Breaking params
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BreakingDeceleration = 1400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovingForwardDeceleration = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DefaultDeceleration = 1100;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxBreakingLength = 2.0f; // Time to brake from max speed to zero

	UPROPERTY(EditAnywhere, Category = "Movement")
	float BreakingFriction = 2.0f; // Additional friction during brake

	FTimerHandle BreakingTimer;
	bool bIsBreaking = false;

	UFUNCTION()
	void ApplyBreakingForce();


	// Turning
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 100.0f;


	//Jump
	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpHeight = 500.f;


	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDClass;



	// Input handlers
	void OnBoostStarted();
	void OnBoostEnded();
	void OnMoveForward();
	void OnStopMovingForward();
	void OnStartBreaking();
	void OnStopBreaking();
	void OnTurnRight();
	void OnTurnLeft();
	void HandleTurning(float TurnInput);
	void OnJump();


private:
	FTimerHandle BoostCooldownTimer;
	FTimerHandle BoostHoldTimer;
	bool bCanBoost = true;
	bool bIsBoosting = false;
	float CurrentBoostTime = 0.f;
	float CurrentSpeedDropoff = 1.f;
	float CurrentBoostHoldTime = 0.f;
	

	

	bool isMovingForward = false;
	bool isBreaking = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


		

};
