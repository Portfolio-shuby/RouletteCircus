// Fill out your copyright notice in the Description page of Project Settings.


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

void APlayerTurnController::Server_BlockBullet_Implementation(APlayerHandState* Target, int32 bReflected)
{
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

void APlayerTurnController::Server_CalculateShootingResults_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(
		TurnEndDelayHandle, this,
		&APlayerTurnController::Server_EndMyTurn,
		4.0f, false
	);
}

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