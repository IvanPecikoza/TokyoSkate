// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "W_SkateHUD.generated.h"


/**
 * 
 */
UCLASS()
class TOKYOSKATE_API UW_SkateHUD : public UUserWidget
{
	GENERATED_BODY()


public:
    // Designer-exposed elements
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Time;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_TotalScore;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Current_Score;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Multiplier;

    void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
