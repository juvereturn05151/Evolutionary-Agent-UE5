// Fill out your copyright notice in the Description page of Project Settings.


#include "PopulationManager.h"
#include "EvolutionAgent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APopulationManager::APopulationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APopulationManager::BeginPlay()
{
	Super::BeginPlay();
    SpawnInitialPopulation();
}

// Called every frame
void APopulationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APopulationManager::SpawnInitialPopulation()
{
    // Debug 1: Verify function is called
    UE_LOG(LogTemp, Warning, TEXT("SpawnInitialPopulation() called"));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Attempting to spawn agents"));

    if (!AgentClass) 
    {
        // Debug 2: Check AgentClass failure
        UE_LOG(LogTemp, Error, TEXT("No AgentClass assigned!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Missing Agent Class"));
        return;
    }

    for (int32 i = 0; i < InitialPopulation; i++)
    {
        // Calculate random position
        FVector Location = GetActorLocation() +
            FVector(
                FMath::FRandRange(-SpawnArea.X / 2, SpawnArea.X / 2),
                FMath::FRandRange(-SpawnArea.Y / 2, SpawnArea.Y / 2),
                0
            );

        // Random rotation
        FRotator Rotation(0, 0, 0);

        // Spawn parameters
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        // Spawn the agent
        GetWorld()->SpawnActor<AEvolutionAgent>(
            AgentClass,
            Location,
            Rotation,
            SpawnParams
        );
    }
}

