// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/ScoreManager.h"

// Sets default values
AScoreManager::AScoreManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AScoreManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AScoreManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScoreManager::AddScore(int32 Points)
{
    // Reset combo timer
    GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AScoreManager::EndCombo, 3.0f, false);

    // Apply score with multiplier
    CurrentComboScore += Points * ComboMultiplier;
    ComboMultiplier += 0.2f;

    OnScoreUpdated.Broadcast(CurrentComboScore, TotalScore);

    UE_LOG(LogTemp, Warning, TEXT("Add Score Executed, total = %d"),CurrentComboScore);
}

void AScoreManager::EndCombo()
{
    TotalScore += CurrentComboScore;
    CurrentComboScore = 0;
    ComboMultiplier = 1.0f;
    OnScoreUpdated.Broadcast(0, TotalScore);
}

void AScoreManager::UpdateGameTimer()
{
    UE_LOG(LogTemp, Warning, TEXT("Update Called"));
    RemainingGameTime = FMath::Max(0.f, RemainingGameTime -= 1);

    if (RemainingGameTime <= 0.f)
    {
        GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);
        EndGame();
    }
    /*else
    {
        GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &AScoreManager::UpdateGameTimer, 1.f, true);
    }*/
}


void AScoreManager::EndGame()
{
    EndCombo();
}



void AScoreManager::Init()
{
    UE_LOG(LogTemp, Warning, TEXT("Init for Manager"));

    StartTime = GetWorld()->GetTimeSeconds();
    RemainingGameTime = GameDuration;

    GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &AScoreManager::UpdateGameTimer, 1.f, true);
}