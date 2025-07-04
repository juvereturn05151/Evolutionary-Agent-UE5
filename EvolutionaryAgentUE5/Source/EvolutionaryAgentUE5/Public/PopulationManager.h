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

	// Spawn area dimensions
	UPROPERTY(EditAnywhere, Category = "Evolution")
	FVector SpawnArea = FVector(1000, 1000, 0);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnInitialPopulation();

};
