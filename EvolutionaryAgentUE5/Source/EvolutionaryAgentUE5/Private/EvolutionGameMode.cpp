#include "EvolutionGameMode.h"
#include "EvolutionPlayerController.h"

AEvolutionGameMode::AEvolutionGameMode()
{
    PlayerControllerClass = AEvolutionPlayerController::StaticClass();
    // No pawn needed for pure click interaction
    DefaultPawnClass = nullptr; 
}