// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EvolutionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EVOLUTIONARYAGENTUE5_API AEvolutionPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    void OnClick();
};
