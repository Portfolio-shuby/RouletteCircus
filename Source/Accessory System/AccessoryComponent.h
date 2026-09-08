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
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accessory")
    UDataTable* AccessoryTable;

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
