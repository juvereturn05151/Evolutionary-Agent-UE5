// Fill out your copyright notice in the Description page of Project Settings.


#include "PopulationManager.h"
#include "EvolutionAgent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

APopulationManager* APopulationManager::Instance = nullptr;

APopulationManager* APopulationManager::GetInstance(const UObject* WorldContextObject)
{
    if (!Instance)
    {
        UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
        if (World)
        {
            // Find existing instance in the world
            Instance = Cast<APopulationManager>(UGameplayStatics::GetActorOfClass(World, APopulationManager::StaticClass()));

            // If none exists, spawn one
            if (!Instance)
            {
                FActorSpawnParameters SpawnParams;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                Instance = World->SpawnActor<APopulationManager>(APopulationManager::StaticClass(), FTransform::Identity, SpawnParams);
            }
        }
    }
    return Instance;
}

APopulationManager::APopulationManager()
{
    // Ensure this actor ticks
    PrimaryActorTick.bCanEverTick = true;

    // Make sure this actor persists between levels
    bReplicates = true;
    bAlwaysRelevant = true;
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
	ElapsedTime += DeltaTime;
    if (ElapsedTime >= TrialTime) 
    {
		//Breed New Population
		ElapsedTime = 0.0f; // Reset elapsed time
    }
}

void APopulationManager::SpawnInitialPopulation()
{
    if (!AgentClass) 
    {
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
        AEvolutionAgent* NewAgent = GetWorld()->SpawnActor<AEvolutionAgent>(
            AgentClass,
            Location,
            Rotation,
            SpawnParams
        );

        if (NewAgent)
        {
            // Generate random color (RGB 0-255)
            FColor RandomColor(
                FMath::RandRange(0, 255),   // Red
                FMath::RandRange(0, 255),   // Green
                FMath::RandRange(0, 255)     // Blue
            );

            // Set the random color
            NewAgent->SetEvolvedColor(RandomColor);

            // Optional: Apply traits immediately
            NewAgent->ApplyTraits();

            Population.Add(NewAgent);
        }
    }
}

void APopulationManager::AddToPopulation(AEvolutionAgent* NewAgent)
{
    if (NewAgent)
    {
        Population.Add(NewAgent);  // Add to the array
    }
}

