#include "EvolutionPlayerController.h"
#include "TopDownCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EvolutionAgent.h"
#include "Engine/World.h"

void AEvolutionPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }

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

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &AEvolutionPlayerController::HandleClick);
    }
}

void AEvolutionPlayerController::HandleClick(const FInputActionValue& Value)
{
    // 1. Use custom object channel
    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic); // Or your custom channel

    // 2. More precise trace
    FHitResult Hit;
    GetHitResultUnderCursorByChannel(
        ETraceTypeQuery::TraceTypeQuery1, // Use your custom trace channel
        true, // Trace complex
        Hit
    );

    // 3. Debug output
    if (Hit.bBlockingHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *Hit.GetActor()->GetName());
        DrawDebugSphere(GetWorld(), Hit.Location, 20.f, 12, FColor::Red, false, 2.f);

        if (Hit.GetActor()->IsA<AEvolutionAgent>())
        {
            Hit.GetActor()->Destroy();
            UE_LOG(LogTemp, Warning, TEXT("Agent destroyed!"));
        }
    }
}