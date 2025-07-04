#include "EvolutionGameMode.h"
#include "EvolutionPlayerController.h"

AEvolutionGameMode::AEvolutionGameMode()
{
    PlayerControllerClass = AEvolutionPlayerController::StaticClass();
    DefaultPawnClass = nullptr; // No pawn needed for pure click interaction
}