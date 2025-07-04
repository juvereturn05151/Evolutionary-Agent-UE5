#include "TopDownCameraActor.h"
#include "Camera/CameraComponent.h"

ATopDownCameraActor::ATopDownCameraActor()
{
    PrimaryActorTick.bCanEverTick = false;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    RootComponent = CameraComponent;

    // Configure top-down view
    CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
    CameraComponent->SetOrthoWidth(2000.f);
    CameraComponent->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
}