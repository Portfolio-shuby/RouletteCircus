#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AccessoryTypes.generated.h"

//장신구를 장착할 수 있는 부위(Socket)에 대응되는 슬롯들
UENUM(BlueprintType)
enum class EAccessorySlot : uint8
{
    Head,
    Face,
    LeftFinger,
    RightFinger,
    Neck
};

//장신구 데이터
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

//장착한 장신구를 표현하는 구조체
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
