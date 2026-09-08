
#include "PlayerGameMode.h"
#include "GameModes/RC_GameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Actor/ReloadMachine.h"
#include "EngineUtils.h"

void APlayerGameMode::StartTurn()
{
	Turns++;

	if (Turns == 1)
	{
		CurrentTurnIndex = FMath::RandRange(0, PlayerList.Num() - 1);
	}

	APlayerTurnController* CurrentPC = PlayerList[CurrentTurnIndex];
	if (CurrentPC)
	{
		for (APlayerTurnController* PC : PlayerList)
		{
			if (PC && PC != CurrentPC)
			{
				PC->bIsMyTurn = false;
				PC->Server_StartMyTurn(false);
			}
		}

		CurrentPC->bIsMyTurn = true;

		FTimerHandle StartHandle;
		TWeakObjectPtr<APlayerTurnController> WeakPC = CurrentPC;
		GetWorldTimerManager().SetTimer(
			StartHandle,
			[WeakPC]() {
				if (WeakPC.IsValid())
					WeakPC->Server_StartMyTurn(true);
			},
			2.0f, false
		);
	}
}

void APlayerGameMode::EndTurn()
{
	CurrentTurnIndex = (CurrentTurnIndex + 1) % PlayerList.Num();

	while (PlayerList[CurrentTurnIndex] == nullptr)
	{
		CurrentTurnIndex = (CurrentTurnIndex + 1) % PlayerList.Num();
	}

	StartTurn();
}
