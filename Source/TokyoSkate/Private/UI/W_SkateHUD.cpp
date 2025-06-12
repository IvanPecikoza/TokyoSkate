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
            if (Manager->GetRemainingGameTime() >= 0) 
            {
                Text_Restart->SetVisibility(ESlateVisibility::Hidden);
                Text_TotalScore->SetText(FText::AsNumber(Manager->GetTotalScore()));
                Text_Current_Score->SetText(FText::AsNumber(Manager->GetCurrentComboScore()));
                Text_Multiplier->SetText(FText::AsNumber(Manager->GetMultiplier()));
                Text_Time->SetText(FText::AsNumber((Manager->GetRemainingGameTime())));
            }
            else
            {
                Text_Restart->SetVisibility(ESlateVisibility::Visible);
            }
            
        }
    }
}



