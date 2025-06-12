// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/TimeTrialGameMode.h"


ATimeTrialGameMode::ATimeTrialGameMode()
{
    // Create and attach ScoreManager
    ScoreManager = CreateDefaultSubobject<AScoreManager>(TEXT("ScoreManager"));
}

void ATimeTrialGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (ScoreManager)
    {
        ScoreManager->Init();
    }
}