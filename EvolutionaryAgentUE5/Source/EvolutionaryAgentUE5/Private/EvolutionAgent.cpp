// Fill out your copyright notice in the Description page of Project Settings.

#include "EvolutionAgent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AEvolutionAgent::AEvolutionAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// Add basic cube mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMesh.Succeeded())
	{
		Mesh->SetStaticMesh(CubeMesh.Object);
	}

	// Default traits
	EvolvedColor = FLinearColor::Green;
	EvolvedSize = FVector(1.0f);
}

// Called when the game starts or when spawned
void AEvolutionAgent::BeginPlay()
{
	Super::BeginPlay();
	ApplyTraits();
}

// Called every frame
void AEvolutionAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEvolutionAgent::SetEvolvedColor(const FLinearColor& NewColor)
{
	EvolvedColor = NewColor;
}

void AEvolutionAgent::SetEvolvedSize(const FVector& NewSize)
{
	EvolvedSize = NewSize;
}

FLinearColor& AEvolutionAgent::GetEvolvedColor()
{
	return EvolvedColor;
}

FVector& AEvolutionAgent::GetEvolvedSize()
{
	return EvolvedSize;
}

void AEvolutionAgent::ApplyTraits()
{
	// Set size
	SetActorScale3D(EvolvedSize);

	// Set color
	UMaterialInterface* BaseMaterial = Mesh->GetMaterial(0);
	if (BaseMaterial)
	{
		UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		DynMat->SetVectorParameterValue("Color", EvolvedColor);
		Mesh->SetMaterial(0, DynMat);
	}
}

