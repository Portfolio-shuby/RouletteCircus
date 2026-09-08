// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHandState.h"
#include "PlayerTurnController.h"
#include "SLobbyPlayerController.h"
#include "../AI/AIPlayerController.h"
#include "../AI/PlayerControllerVerAI.h"
#include "../AI/AIGameMode.h"
#include "TimerManager.h"

void APlayerHandState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerHandState, Life);
}

void APlayerHandState::UpdateLife_Implementation(int32 Num)
{
	Life = FMath::Clamp(Life - 1, 0, MaxLife);

	if (HasAuthority())
	{
		OnRep_Life();
	}
}

void APlayerHandState::OnRep_Life()
{
	AGameStateBase* CurrentGameState = GetWorld()->GetGameState();
	if (AAIModeGameUpdater* AIModeGame = Cast<AAIModeGameUpdater>(CurrentGameState))
	{
		AIModeGame->BroadcastUpdateLife();
	}
	else if (AMyGameState* MultiGame = Cast<AMyGameState>(CurrentGameState))
	{
		MultiGame->BroadcastUpdateLife();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlayerHandState::OnRep_Life);
	}
}