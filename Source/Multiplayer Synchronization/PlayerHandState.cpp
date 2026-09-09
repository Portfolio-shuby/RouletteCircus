
#include "PlayerHandState.h"
#include "PlayerTurnController.h"
#include "SLobbyPlayerController.h"
#include "../AI/AIPlayerController.h"
#include "../AI/PlayerControllerVerAI.h"
#include "../AI/AIGameMode.h"
#include "TimerManager.h"

//변수 Replicate 설정
void APlayerHandState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerHandState, Life);
}

//체력 회복, 감소 등에 따라 Life 변수 수정
void APlayerHandState::UpdateLife_Implementation(int32 Num)
{
	Life = FMath::Clamp(Life - 1, 0, MaxLife);

	if (HasAuthority())
	{
		OnRep_Life();
	}
}

//Life 변수의 Server에서의 상태 변경이 Client에 동기화
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
