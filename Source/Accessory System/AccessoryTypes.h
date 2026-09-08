#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AccessoryTypes.generated.h"

UENUM(BlueprintType)
enum class EAccessorySlot : uint8
{
    Head,
    Face,
    LeftFinger,
    RightFinger,
    Neck
};

USTRUCT(BlueprintType)
struct FAccessoryData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EAccessorySlot Slot;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UMaterialInterface* Material;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector RelativeLocation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRotator RelativeRotation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector RelativeScale;
};

USTRUCT(BlueprintType)
struct FEquippedAccessory
{
    GENERATED_BODY()

	//Equipped Accessory Name
    UPROPERTY()
    FName AccessoryRow;

	//Equipped Accessory Mesh Component
    UStaticMeshComponent* MeshComponent = nullptr;
};