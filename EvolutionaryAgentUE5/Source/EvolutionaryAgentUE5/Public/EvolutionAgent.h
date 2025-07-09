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
	//collision component (MUST be root)
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CollisionComponent;

	//visible box component
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	float timeToDie;

	bool dead = false;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent Trait")
	FColor EvolvedColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent Trait")
	FVector EvolvedSize;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetEvolvedColor(const FColor& NewColor);
	void SetEvolvedSize(const FVector& NewSize);
	void SetTimeToDie(float NewTime);

	FColor& GetEvolvedColor();
	FVector& GetEvolvedSize();
	float GetTimeToDie() const;

	void ApplyTraits();
	void DisableComponents();

};
