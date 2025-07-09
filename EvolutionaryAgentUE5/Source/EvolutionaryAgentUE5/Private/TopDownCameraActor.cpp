#include "TopDownCameraActor.h"
#include "Camera/CameraComponent.h"

ATopDownCameraActor::ATopDownCameraActor()
{
    PrimaryActorTick.bCanEverTick = false;

    cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    RootComponent = cameraComponent;

    //configure top-down view
    cameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
    cameraComponent->SetOrthoWidth(2000.f);
    cameraComponent->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
}