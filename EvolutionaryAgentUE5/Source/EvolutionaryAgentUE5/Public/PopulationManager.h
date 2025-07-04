// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PopulationManager.generated.h"

UCLASS()
class EVOLUTIONARYAGENTUE5_API APopulationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APopulationManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Agent class to spawn
	UPROPERTY(EditAnywhere, Category = "Evolution")
	TSubclassOf<class AEvolutionAgent> AgentClass;

	// Number of agents to spawn
	UPROPERTY(EditAnywhere, Category = "Evolution")
	int32 InitialPopulation = 50;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Population Manager", meta = (AllowPrivateAccess = "true"))
	int32 TrialTime = 10;  // Time in seconds for each trial
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Population Manager", meta = (AllowPrivateAccess = "true"))
	int32 Generation = 1;
	float ElapsedTime = 0.0f;

	// Spawn area dimensions
	UPROPERTY(EditAnywhere, Category = "Evolution")
	FVector SpawnArea = FVector(1000, 1000, 0);

	UPROPERTY()  // Makes it visible to UE's reflection system
	TArray<AEvolutionAgent*> Population;  // List of agent pointers

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnInitialPopulation();

	void AddToPopulation(AEvolutionAgent* NewAgent);

	UFUNCTION(BlueprintCallable)
	int32 GetGeneration() const { return Generation; }

	UFUNCTION(BlueprintCallable)
	float GetElapsedTime() const { return ElapsedTime; }


};
