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


	UPROPERTY(BlueprintReadOnly, Category = "Scoring")
	float RemainingGameTime = 60.f;

	UPROPERTY(BlueprintReadOnly, Category = "Scoring")
	float StartTime = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Scoring")
	float GameDuration = 60.f;



	UFUNCTION()
	void UpdateGameTimer();

	void EndCombo();
	void EndGame();

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Add these getters
	UFUNCTION(BlueprintCallable, Category = "Scoring")
	int32 GetTotalScore() const { return TotalScore; }

	UFUNCTION(BlueprintCallable, Category = "Scoring")
	int32 GetCurrentComboScore() const { return CurrentComboScore; }

	UFUNCTION(BlueprintCallable, Category = "Scoring")
	float GetMultiplier() const { return ComboMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "Scoring")
	float GetRemainingGameTime() const { return RemainingGameTime; }


	void Init();


};
