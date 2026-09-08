
#include "PlayerTurnController.h"
#include "PlayerGameMode.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "Roulette_Circus/UI/MainHUD.h"
#include "../GameModes/MyGameState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Algo/RandomShuffle.h"
#include "Math/UnrealMathUtility.h"
#include "UI/Info_Widget.h"
#include "GameModes/SGameModeBase.h"
#include "../GameModes/RC_GameInstance.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/SpotLight.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Data/PointManager.h"

//사격 판정을 최종 계산하는 Server_BlockBullet()에서 조건에 따라 턴 종료
void APlayerTurnController::Server_BlockBullet_Implementation(APlayerHandState* Target, int32 bReflected)
{
	//방어 성공 + 반사
	if (!bCanBlock)
	{
		if (Target == PS || bReflected % 2 == 1)
		{
			bIsSuicide = true;

			if (Targets.Num() < 2)
			{
				while (Targets.Num() < 2)
				{
					Targets.Add(PS);
				}
			}

			//자살로 턴 종료
			GetWorld()->GetTimerManager().SetTimer(
				DelayHandle3, this,
				&APlayerTurnController::Server_CalculateShootingResults,
				1.0f, false
			);
		}
	}

	//방어 실패, 일반적인 턴 종료
	if (bReflected == 0 && !bIsSuicide)
	{
		ClickCount++;

		if (ClickCount >= 2)
		{
			//턴을 모두 진행해서 턴 종료
			GetWorld()->GetTimerManager().SetTimer(
				DelayHandle3, this,
				&APlayerTurnController::Server_CalculateShootingResults,
				1.0f, false
			);
		}
		else
		{
			Server_SetbCanTargeting(false);
		}
	}
}

//턴의 진행 결과에 대한 최종적인 정산을 한 뒤, EndMyTurn()을 실행하여 안정적인 턴 전환
void APlayerTurnController::Server_CalculateShootingResults_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(
		TurnEndDelayHandle, this,
		&APlayerTurnController::Server_EndMyTurn,
		4.0f, false
	);
}

//실제 턴 종료를 담당하는 EndMyTurn()에서 State 초기화, 다음 플레이어 탐색, 다음 턴 시작 전 작업 처리 등을 진행
void APlayerTurnController::EndMyTurn()
{
	Server_SetPhaseData(0, false, false, false, nullptr, false, 0);
	ClickCount = 0;
	Server_SetForcedShootTarget(-3);
	Server_TriggerBlur(false);

	APlayerGameMode* GM = Cast<APlayerGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		APlayerTurnController* NextPC = nullptr;

		int32 PlayerNum = GM->PlayerList.Num();

		//다른 플레이어들 중 사망하지 않은 다음 번호의 플레이어 탐색
		for (int i = 1; i < PlayerNum; i++)
		{
			APlayerTurnController* TmpNextPC = GM->PlayerList[(OwnNum + i) % PlayerNum];

			if (!IsValid(TmpNextPC))
				continue;

			APlayerHandState* NextPS = TmpNextPC->GetPlayerState<APlayerHandState>();

			if (!IsValid(NextPS))
				continue;

			if (!NextPS->bIsDead)
			{
				NextPC = TmpNextPC;
				break;
			}
		}

		if (!IsValid(NextPC))
			return;

		//다음 플레이어의 턴 시작 전 몇 가지 전처리 과정
		if (IsValid(NextPC->RldM))
			NextPC->RldM->Server_RunMachine(true);

		if (PS->Keywords[K(Dynamite)] > 0)
		{
			NextPC->Server_UpdateKeywords(K(Dynamite), PS->Keywords[K(Dynamite)]);
			Server_UpdateKeywords(K(Dynamite), -PS->Keywords[K(Dynamite)]);

			if (DynM)
				DynM->Multicast_Deactivate();
		}

		GM->EndTurn();
	}
}
