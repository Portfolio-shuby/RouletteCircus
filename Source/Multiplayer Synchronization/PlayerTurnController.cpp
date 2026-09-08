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

void APlayerTurnController::Server_Shoot_Implementation()
{
    Shoot();
}

void APlayerTurnController::Shoot()
{

    Server_BlockBullet();

}

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