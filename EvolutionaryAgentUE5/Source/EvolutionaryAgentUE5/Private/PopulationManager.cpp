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

	//make sure this actor persists between levels, however, we will not use this feature in this example
   /* bReplicates = true;
    bAlwaysRelevant = true;*/
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
        BreedNewPopulation();
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
        //calculate random position
        FVector Location = GetActorLocation() +
            FVector(
                FMath::FRandRange(-SpawnArea.X / 2, SpawnArea.X / 2),
                FMath::FRandRange(-SpawnArea.Y / 2, SpawnArea.Y / 2),
                0
            );

        //fixed rotation
        FRotator Rotation(0, 0, 0);

        //spawn parameters
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        //spawn the agent
        AEvolutionAgent* NewAgent = GetWorld()->SpawnActor<AEvolutionAgent>(
            AgentClass,
            Location,
            Rotation,
            SpawnParams
        );

        if (NewAgent)
        {
            //generate random color (RGB 0-255)
            FColor RandomColor(
                FMath::RandRange(0, 255),   // Red
                FMath::RandRange(0, 255),   // Green
                FMath::RandRange(0, 255)     // Blue
            );

            //set the random color
            NewAgent->SetEvolvedColor(RandomColor);

            //apply traits immediately
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

AEvolutionAgent* APopulationManager::Breed(AEvolutionAgent* Parent1, AEvolutionAgent* Parent2)
{
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
    AEvolutionAgent* OffSpring = GetWorld()->SpawnActor<AEvolutionAgent>(
        AgentClass,
        Location,
        Rotation,
        SpawnParams
    );

    if (OffSpring) 
    {
        if (FMath::RandRange(0, 1000) > 5)
        {
            FColor Color(
                FMath::RandRange(0, 10) < 5 ? Parent1->GetEvolvedColor().R : Parent2->GetEvolvedColor().R,   // Red
                FMath::RandRange(0, 10) < 5 ? Parent1->GetEvolvedColor().G : Parent2->GetEvolvedColor().G,   // Green
                FMath::RandRange(0, 10) < 5 ? Parent1->GetEvolvedColor().B : Parent2->GetEvolvedColor().B     // Blue
            );

            OffSpring->SetEvolvedColor(Color);
        }
        else
        {
            FColor RandomColor(
                FMath::RandRange(0, 255),   // Red
                FMath::RandRange(0, 255),   // Green
                FMath::RandRange(0, 255)     // Blue
            );

            OffSpring->SetEvolvedColor(RandomColor);
        }

        OffSpring->ApplyTraits();
    }

    return OffSpring;
}

void APopulationManager::BreedNewPopulation()
{
    TArray<AEvolutionAgent*> NewPopulation;

    // 1. Create sorted copy of the population (descending by TimeToDie)
    TArray<AEvolutionAgent*> SortedList = Population;
    SortedList.Sort([](const AEvolutionAgent& A, const AEvolutionAgent& B) {
        return A.GetTimeToDie() < B.GetTimeToDie(); // Descending sort
        });

    // 2. Clear old population (but don't destroy yet - we need parents for breeding)
    Population.Empty();

    // 3. Breed upper half of the sorted list
    const int32 HalfPopulation = SortedList.Num() / 2;
    for (int32 i = HalfPopulation; i < SortedList.Num() - 1; i++)
    {
        for (int32 j = HalfPopulation + 1; j < SortedList.Num(); j++)
        {
            // Breed in both orders (A+B and B+A)
            if (AEvolutionAgent* Child1 = Breed(SortedList[i], SortedList[j]))
            {
                NewPopulation.Add(Child1);
            }
            if (AEvolutionAgent* Child2 = Breed(SortedList[j], SortedList[i]))
            {
                NewPopulation.Add(Child2);
            }
        }
    }

    // 4. Destroy all parents and previous population
    for (AEvolutionAgent* Agent : SortedList)
    {
        if (IsValid(Agent))
        {
            Agent->Destroy();
        }
    }

    // 5. Update the population and generation counter
    Population = NewPopulation;
    Generation++;

    UE_LOG(LogTemp, Log, TEXT("New generation %d created with %d agents"), Generation, Population.Num());
}

