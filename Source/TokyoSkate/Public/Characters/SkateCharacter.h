// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SkateCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class TOKYOSKATE_API ASkateCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASkateCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

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
	float BoostCooldown = 0.9f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinBoostDuration = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxBoostDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BoostPower = 3000.f;


	//Breaking params
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BreakingDeceleration = 1400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovingForwardDeceleration = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DefaultDeceleration = 1100;


	// Turning
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 100.0f;


	//Jump
	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpHeight = 500.f;


	// Input handlers
	void OnBoostStarted();
	void TestRawInput();
	void OnBoostEnded();
	void OnMoveForward();
	void OnStopMovingForward();
	void OnStartBreaking();
	void OnStopBreaking();
	void OnTurnRight();
	void OnTurnLeft();
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
