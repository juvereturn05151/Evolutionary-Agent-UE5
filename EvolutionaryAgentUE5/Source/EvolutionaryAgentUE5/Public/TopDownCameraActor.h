// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopDownCameraActor.generated.h"

UCLASS()
class EVOLUTIONARYAGENTUE5_API ATopDownCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATopDownCameraActor();

private:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;
};
