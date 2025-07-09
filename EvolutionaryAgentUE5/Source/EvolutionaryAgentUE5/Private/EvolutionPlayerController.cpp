#include "EvolutionPlayerController.h"
#include "TopDownCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EvolutionAgent.h"
#include "PopulationManager.h"
#include "Engine/World.h"

void AEvolutionPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(defaultMappingContext, 0);
    }

    //enable mouse controls
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    //find camera actor in level
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

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(leftClickAction, ETriggerEvent::Triggered, this, &AEvolutionPlayerController::HandleClick);
    }
}

void AEvolutionPlayerController::HandleClick(const FInputActionValue& Value)
{
    //1. use custom object channel
    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic); // Or your custom channel

    //2. more precise trace
    FHitResult Hit;
    GetHitResultUnderCursorByChannel(
        ETraceTypeQuery::TraceTypeQuery1, // Use your custom trace channel
        true, // Trace complex
        Hit
    );

    //3. debug output
    if (Hit.bBlockingHit)
    {
        DrawDebugSphere(GetWorld(), Hit.Location, 20.f, 12, FColor::Red, false, 2.f);

        if (Hit.GetActor()->IsA<AEvolutionAgent>())
        {
            if (AEvolutionAgent* Agent = static_cast<AEvolutionAgent*>(Hit.GetActor()))
            {
                Agent->SetTimeToDie(APopulationManager::GetInstance(GetWorld())->GetElapsedTime());
				Agent->DisableComponents();
            }
        }
    }
}