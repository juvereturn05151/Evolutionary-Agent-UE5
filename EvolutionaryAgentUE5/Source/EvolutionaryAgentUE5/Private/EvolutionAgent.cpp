// Fill out your copyright notice in the Description page of Project Settings.

#include "EvolutionAgent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AEvolutionAgent::AEvolutionAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitCapsuleSize(50.f, 50.f); // Radius, HalfHeight
	CollisionComponent->SetCollisionProfileName(TEXT("EvolutionAgent")); // Or "Custom" profile


	// 2. Configure mesh (now attached to collision root)
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Let root handle collisions

	// Add basic cube mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMesh.Succeeded())
	{
		Mesh->SetStaticMesh(CubeMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> BaseMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
	if (BaseMaterial.Succeeded())
	{
		Mesh->SetMaterial(0, BaseMaterial.Object);
	}

	// Default traits
	//SetEvolvedColor(FColor(255, 100, 50));
	EvolvedSize = FVector(1.0f);
}

// Called when the game starts or when spawned
void AEvolutionAgent::BeginPlay()
{
	Super::BeginPlay();
	//ApplyTraits();
}

// Called every frame
void AEvolutionAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEvolutionAgent::SetEvolvedColor(const FColor& NewColor)
{
	EvolvedColor = NewColor;
}

void AEvolutionAgent::SetEvolvedSize(const FVector& NewSize)
{
	EvolvedSize = NewSize;
}

FColor& AEvolutionAgent::GetEvolvedColor()
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
		DynMat->SetVectorParameterValue("Color", FLinearColor(EvolvedColor));
		Mesh->SetMaterial(0, DynMat);
	}
}

