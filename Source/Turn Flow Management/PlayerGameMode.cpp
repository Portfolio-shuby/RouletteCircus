
#include "PlayerGameMode.h"
#include "GameModes/RC_GameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Actor/ReloadMachine.h"
#include "EngineUtils.h"

//GameMode에서 다음 플레이어의 턴을 시작시켜 줌으로서, GameMode에서 턴의 전환을 관리
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
		//턴이 아닌 플레이어 처리
		for (APlayerTurnController* PC : PlayerList)
		{
			if (PC && PC != CurrentPC)
			{
				PC->bIsMyTurn = false;
				PC->Server_StartMyTurn(false);
			}
		}
		
		CurrentPC->bIsMyTurn = true;

		//턴인 플레이어 처리
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

//GameMode에서 턴의 종료를 최종적으로 처리하고 다음 플레이어의 턴을 시작하는 턴 전환 관리
void APlayerGameMode::EndTurn()
{
	CurrentTurnIndex = (CurrentTurnIndex + 1) % PlayerList.Num();

	while (PlayerList[CurrentTurnIndex] == nullptr)
	{
		CurrentTurnIndex = (CurrentTurnIndex + 1) % PlayerList.Num();
	}

	StartTurn();
}
