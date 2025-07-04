#include "EvolutionPlayerController.h"
#include "TopDownCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "EvolutionAgent.h"
#include "Engine/World.h"

void AEvolutionPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Enable mouse controls
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    // Find camera actor in level
    TArray<AActor*> Cameras;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopDownCameraActor::StaticClass(), Cameras);

    if (Cameras.Num() > 0)
    {
        SetViewTargetWithBlend(Cameras[0], 0.5f);
    }
}

void AEvolutionPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("LeftClick", IE_Pressed, this, &AEvolutionPlayerController::OnClick);
}

void AEvolutionPlayerController::OnClick()
{
    UE_LOG(LogTemp, Warning, TEXT("Fuck"));

    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Clicked on: %s"), *Hit.GetActor()->GetName());

        if (AEvolutionAgent* Agent = Cast<AEvolutionAgent>(Hit.GetActor()))
        {
            Agent->Destroy();
            UE_LOG(LogTemp, Log, TEXT("Agent killed!"));
        }
    }
}