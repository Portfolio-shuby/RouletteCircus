// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AccessoryTypes.h"
#include "AccessoryComponent.generated.h"

class UStaticMeshComponent;
class UDataTable;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROULETTE_CIRCUS_API UAccessoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAccessoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	//장신구 데이터가 저장된 DataTable
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accessory")
    UDataTable* AccessoryTable;

	//현재 장착된 장신구들을 (Slot, 장신구) 형태로 저장하는 Map
    UPROPERTY()
    TMap<EAccessorySlot, FEquippedAccessory> EquippedAccessories;

public:

    UFUNCTION(BlueprintCallable)
    void Equip(FName AccessoryRow);

    UFUNCTION(BlueprintCallable)
    void Unequip(EAccessorySlot Slot);

private:

    UStaticMeshComponent* CreateAccessory(const FAccessoryData& Data);

    FName GetSocketName(EAccessorySlot Slot) const;

    USkeletalMeshComponent* GetCharacterMesh() const;

		
};
