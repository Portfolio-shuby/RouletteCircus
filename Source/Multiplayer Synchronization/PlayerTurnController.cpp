
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

//Server RPC를 통한 사격 요청
void APlayerTurnController::OnShoot()
{
    if (HasAuthority())
    {
        Shoot();
    }
    else
    {
        Server_Shoot();
    }
}

//Client의 사격을 Server RPC를 통해 Server로 전달
void APlayerTurnController::Server_Shoot_Implementation()
{
    Shoot();
}

//실제 사격 처리는 Server에서 수행하도록 구성
void APlayerTurnController::Shoot()
{

    Server_BlockBullet();

}

//Server에서 대상의 PlayerState를 확인하고 카드 효과 및 사격 결과를 판정한 후, 최종 결과에 따라 State를 변경
void APlayerTurnController::Server_BlockBullet_Implementation(APlayerHandState* Target, int32 bReflected)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerTurnController* PC = Cast<APlayerTurnController>(It->Get());
		if (PC == nullptr) continue;

		APlayerHandState* HandState = PC->GetPlayerState<APlayerHandState>();

		if (HandState->OwnPlayerNum == Target->OwnPlayerNum)
		{
			TargetPC = PC;
			break;
		}
	}

	Target->UpdateLife(-1);
}
