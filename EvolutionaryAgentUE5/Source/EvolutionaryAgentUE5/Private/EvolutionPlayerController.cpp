#include "EvolutionPlayerController.h"
#include "EvolutionAgent.h"
#include "Engine/World.h"

void AEvolutionPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Enable mouse controls
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void AEvolutionPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("LeftClick", IE_Pressed, this, &AEvolutionPlayerController::OnClick);
}

void AEvolutionPlayerController::OnClick()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        if (AEvolutionAgent* Agent = Cast<AEvolutionAgent>(Hit.GetActor()))
        {
            Agent->Destroy();
            UE_LOG(LogTemp, Log, TEXT("Agent killed!"));
        }
    }
}