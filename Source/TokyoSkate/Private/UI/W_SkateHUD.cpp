// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_SkateHUD.h"
#include "Gameplay/ScoreManager.h"
#include <Gameplay/TimeTrialGameMode.h>


void UW_SkateHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    if (ATimeTrialGameMode* GameMode = Cast<ATimeTrialGameMode>(GetWorld()->GetAuthGameMode()))
    {
        if (const AScoreManager* Manager = GameMode->ScoreManager)
        {
            //const float TimeRemaining = Manager->GetRemainingGameTime();
            /*Text_Time->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"),
                FMath::FloorToInt(TimeRemaining / 60.f),
                FMath::FloorToInt(FMath::Fmod(TimeRemaining, 60.f))));*/

            Text_TotalScore->SetText(FText::AsNumber(Manager->GetTotalScore()));
            Text_Current_Score->SetText(FText::AsNumber(Manager->GetCurrentComboScore()));
            Text_Multiplier->SetText(FText::AsNumber(Manager->GetMultiplier()));
            Text_Time->SetText(FText::AsNumber((Manager->GetRemainingGameTime())));
        }
    }
}



