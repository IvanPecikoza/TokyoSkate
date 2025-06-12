// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScoreUpdated, int32, CurrentScore, int32, TotalScore);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOKYOSKATE_API AScoreManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoreManager();

	void AddScore(int32 Points);
	FOnScoreUpdated OnScoreUpdated;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int32 TotalScore = 0;
	int32 CurrentComboScore = 0;
	float ComboMultiplier = 1.0f;

	FTimerHandle ComboTimerHandle;
	FTimerHandle GameTimerHandle;

	void EndCombo();
	void EndGame();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
