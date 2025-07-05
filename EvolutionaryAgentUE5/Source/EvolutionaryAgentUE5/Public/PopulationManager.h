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

	int32 Generation = 1;
	float TrialTime = 10;
	float ElapsedTime = 0.0f;

	//list of agent pointers
	TArray<AEvolutionAgent*> Population;

protected:
	//agent class to spawn
	UPROPERTY(EditAnywhere, Category = "Evolution")
	TSubclassOf<AEvolutionAgent> AgentClass;

	//number of agents to spawn
	UPROPERTY(EditAnywhere, Category = "Evolution")
	int32 InitialPopulation = 50;

	//spawn area dimensions
	UPROPERTY(EditAnywhere, Category = "Evolution")
	FVector SpawnArea = FVector(1000, 1000, 0);

private:
	void AddToPopulation(AEvolutionAgent* NewAgent);

	void BreedNewPopulation();

	AEvolutionAgent* Breed(AEvolutionAgent* Parent1, AEvolutionAgent* Parent2);

protected:
	//called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnInitialPopulation();

	UFUNCTION(BlueprintCallable)
	int32 GetGeneration() const { return Generation; }

	UFUNCTION(BlueprintCallable)
	float GetElapsedTime() const { return ElapsedTime; }


};
