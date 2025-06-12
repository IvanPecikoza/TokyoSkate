// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Obstacle2.h"

// Sets default values
AObstacle2::AObstacle2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AObstacle2::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AObstacle2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObstacle2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacle2::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    UE_LOG(LogTemp, Warning, TEXT("Overlap detected"));
}

