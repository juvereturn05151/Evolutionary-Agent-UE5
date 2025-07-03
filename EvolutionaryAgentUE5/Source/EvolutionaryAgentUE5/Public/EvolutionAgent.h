// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EvolutionAgent.generated.h"

UCLASS()
class EVOLUTIONARYAGENTUE5_API AEvolutionAgent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEvolutionAgent();

private:
	// Visible box component
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent Trait")
	FLinearColor EvolvedColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent Trait")
	FVector EvolvedSize;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetEvolvedColor(const FLinearColor& NewColor);
	void SetEvolvedSize(const FVector& NewSize);

	FLinearColor& GetEvolvedColor();
	FVector& GetEvolvedSize();

	void ApplyTraits();

};
