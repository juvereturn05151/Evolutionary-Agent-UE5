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
            //find existing instance in the world
            Instance = Cast<APopulationManager>(UGameplayStatics::GetActorOfClass(World, APopulationManager::StaticClass()));

            //if none exists, spawn one
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
    //ensure this actor ticks
    PrimaryActorTick.bCanEverTick = true;
}

void APopulationManager::BeginPlay()
{
	Super::BeginPlay();
    SpawnInitialPopulation();
}

void APopulationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	elapsedTime += DeltaTime;
    if (elapsedTime >= trialTime) 
    {
        BreedNewPopulation();
		elapsedTime = 0.0f; 
    }
}

void APopulationManager::SpawnInitialPopulation()
{
    if (!agentClass) 
    {
        UE_LOG(LogTemp, Error, TEXT("No AgentClass assigned!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Missing Agent Class"));
        return;
    }

    for (int32 i = 0; i < initialPopulation; i++)
    {
        AddToPopulation(Breed(NULL, NULL));
    }
}

void APopulationManager::AddToPopulation(AEvolutionAgent* NewAgent)
{
    if (NewAgent)
    {
        population_agents.Add(NewAgent);  // Add to the array
    }
}

AEvolutionAgent* APopulationManager::Breed(AEvolutionAgent* Parent1, AEvolutionAgent* Parent2)
{
    FVector Location = GetActorLocation() +
        FVector(
            FMath::FRandRange(-spawnArea.X / 2, spawnArea.X / 2),
            FMath::FRandRange(-spawnArea.Y / 2, spawnArea.Y / 2),
            0
        );

    //origin rotation
    FRotator Rotation(0, 0, 0);

    //spawn parameters
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    //spawn the agent
    AEvolutionAgent* OffSpring = GetWorld()->SpawnActor<AEvolutionAgent>(
        agentClass,
        Location,
        Rotation,
        SpawnParams
    );

    if (OffSpring) 
    {
        if (FMath::RandRange(0, 1000) > 5 && (Parent1 && Parent2))
        {
            FColor Color(
                FMath::RandRange(0, 10) < 5 ? Parent1->GetEvolvedColor().R : Parent2->GetEvolvedColor().R,  
                FMath::RandRange(0, 10) < 5 ? Parent1->GetEvolvedColor().G : Parent2->GetEvolvedColor().G,  
                FMath::RandRange(0, 10) < 5 ? Parent1->GetEvolvedColor().B : Parent2->GetEvolvedColor().B     
            );

            FVector Size = FMath::RandRange(0, 10) < 5 ? Parent1->GetEvolvedSize() : Parent2->GetEvolvedSize();

            OffSpring->SetEvolvedColor(Color);
            OffSpring->SetEvolvedSize(Size);
        }
        else
        {
            FColor RandomColor(
                FMath::RandRange(0, 255),   // Red
                FMath::RandRange(0, 255),   // Green
                FMath::RandRange(0, 255)     // Blue
            );

            //random size between 1.0 and 2.0
            float size = FMath::RandRange(1.0f, 1.15f);
            FVector RandomSize(
                size,
                size,
                1.0f        
			);

            OffSpring->SetEvolvedColor(RandomColor);
            OffSpring->SetEvolvedSize(RandomSize);
        }

        OffSpring->ApplyTraits();
    }

    return OffSpring;
}

void APopulationManager::BreedNewPopulation()
{
    TArray<AEvolutionAgent*> NewPopulation;

    //1. create sorted copy of the population (descending by TimeToDie)
    TArray<AEvolutionAgent*> SortedList = population_agents;
    SortedList.Sort([](const AEvolutionAgent& A, const AEvolutionAgent& B) {
        return A.GetTimeToDie() < B.GetTimeToDie(); // Descending sort
        });

    //2. clear old population
    population_agents.Empty();

    //3. breed upper half of the sorted list
    const int32 HalfPopulation = SortedList.Num() / 2;
    for (int32 i = HalfPopulation; i < SortedList.Num() - 1; i++)
    {
        for (int32 j = HalfPopulation + 1; j < SortedList.Num(); j++)
        {
            //breed in both orders (A+B and B+A)
            if (AEvolutionAgent* Child1 = Breed(SortedList[i], SortedList[j]))
            {
                NewPopulation.Add(Child1);
                printf("Breed Child 1");
            }
            if (AEvolutionAgent* Child2 = Breed(SortedList[j], SortedList[i]))
            {
                NewPopulation.Add(Child2);
                printf("Breed Child 2");
            }
        }
    }

    //4. destroy all parents and previous population
    for (AEvolutionAgent* Agent : SortedList)
    {
        if (IsValid(Agent))
        {
            Agent->Destroy();
        }
    }

    //5. update the population and generation counter
    population_agents = NewPopulation;
    generation++;
    UE_LOG(LogTemp, Log, TEXT("New generation %d created with %d agents"), generation, population_agents.Num());
}