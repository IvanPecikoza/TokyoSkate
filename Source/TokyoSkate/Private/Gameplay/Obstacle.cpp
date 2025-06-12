// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Obstacle.h"
#include "Components/BoxComponent.h"
#include "Gameplay/ScoreManager.h"
#include <Gameplay/TimeTrialGameMode.h>
#include <Characters/SkateCharacter.h>

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
    TriggerVolume->SetGenerateOverlapEvents(true);
    TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerVolume->SetCollisionObjectType(ECC_WorldDynamic);
    TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


    if (TriggerVolume)
    {
        TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OnOverlapBegin);
        UE_LOG(LogTemp, Warning, TEXT("Overlap event bound successfully"));
        UE_LOG(LogTemp, Warning, TEXT("TriggerVolume generates overlaps: %s"), TriggerVolume->GetGenerateOverlapEvents() ? TEXT("Yes") : TEXT("No"));
        UE_LOG(LogTemp, Warning, TEXT("Collision enabled: %d"), (int32)TriggerVolume->GetCollisionEnabled());

    }
    //AScoreManager* ScoreManager = 
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AObstacle::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    
    UE_LOG(LogTemp, Warning, TEXT("Overlap detected"));
    if (!OtherActor || !bCanTrigger) return;
    
    UE_LOG(LogTemp, Warning, TEXT("Overlap detected with: %s"), *OtherActor->GetName());

    if (OtherActor->IsA<ASkateCharacter>())
    {
        bCanTrigger = false;
        GetWorld()->GetTimerManager().SetTimer(CooldownTimer, [this]() { bCanTrigger = true; }, 1.0f, false);

        if (ATimeTrialGameMode* GameMode = Cast<ATimeTrialGameMode>(GetWorld()->GetAuthGameMode()))
        {
            if (GameMode && GameMode->ScoreManager)
            {
                GameMode->ScoreManager->AddScore(ScoreValue);
                UE_LOG(LogTemp, Warning, TEXT("Added %d points!"), 
                    ScoreValue);
            }
        }
    }
}

