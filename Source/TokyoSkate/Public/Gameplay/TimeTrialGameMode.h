// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ScoreManager.h"
#include "TimeTrialGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOKYOSKATE_API ATimeTrialGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ATimeTrialGameMode();

	virtual void BeginPlay() override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AScoreManager* ScoreManager;
	
};
