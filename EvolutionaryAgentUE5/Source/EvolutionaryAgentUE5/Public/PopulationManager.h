// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PopulationManager.generated.h"

class AEvolutionAgent;

UCLASS()
class EVOLUTIONARYAGENTUE5_API APopulationManager : public AActor
{
	GENERATED_BODY()
	
public:	

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Population", meta = (WorldContext = "WorldContextObject"))
	static APopulationManager* GetInstance(const UObject* WorldContextObject);


private:
	// Sets default values for this actor's properties
	APopulationManager();

	// Static instance pointer
	static APopulationManager* Instance;

	int32 generation = 1;
	float trialTime = 10;
	float elapsedTime = 0.0f;

	//list of agent pointers
	TArray<AEvolutionAgent*> population_agents;

protected:
	//agent class to spawn
	UPROPERTY(EditAnywhere, Category = "Evolution")
	TSubclassOf<AEvolutionAgent> agentClass;

	//number of agents to spawn
	UPROPERTY(EditAnywhere, Category = "Evolution")
	int32 initialPopulation = 50;

	//spawn area dimensions
	UPROPERTY(EditAnywhere, Category = "Evolution")
	FVector spawnArea = FVector(1000, 1000, 0);

private:
	void AddToPopulation(AEvolutionAgent* NewAgent);

	void BreedNewPopulation();

	AEvolutionAgent* Breed(AEvolutionAgent* Parent1, AEvolutionAgent* Parent2);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SpawnInitialPopulation();

	UFUNCTION(BlueprintCallable)
	int32 GetGeneration() const { return generation; }

	UFUNCTION(BlueprintCallable)
	float GetElapsedTime() const { return elapsedTime; }


};
