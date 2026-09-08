// Fill out your copyright notice in the Description page of Project Settings.


#include "Skin/AccessoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DataTable.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/Character.h"

//선택한 장신구의 메쉬 컴포넌트를 생성하여 장착
void UAccessoryComponent::Equip(FName AccessoryRow)
{
    if (!AccessoryTable)
        return;

	//DataTable에서 이름(FName)으로 장신구 Row 탐색
    const FAccessoryData* Data =
        AccessoryTable->FindRow<FAccessoryData>(AccessoryRow, TEXT(""));

    if (!Data)
        return;

    Unequip(Data->Slot);

	//장신구 메쉬 컴포넌트 생성
    UStaticMeshComponent* MeshComponent = CreateAccessory(*Data);

    FEquippedAccessory Equipped;
    Equipped.AccessoryRow = AccessoryRow;
    Equipped.MeshComponent = MeshComponent;

    EquippedAccessories.Add(Data->Slot, Equipped);
}

//지정한 슬롯에 장착된 장신구의 메쉬 컴포넌트를 제거하여 장착 해제
void UAccessoryComponent::Unequip(EAccessorySlot Slot)
{
    if (FEquippedAccessory* Equipped = EquippedAccessories.Find(Slot))
    {
        if (Equipped->MeshComponent)
        {
            Equipped->MeshComponent->DestroyComponent();
        }

        EquippedAccessories.Remove(Slot);
    }
}

//장신구 데이터를 받아 메쉬 컴포넌트를 생성하는 함수
UStaticMeshComponent* UAccessoryComponent::CreateAccessory(const FAccessoryData& Data)
{
    USkeletalMeshComponent* CharacterMesh = GetCharacterMesh();

    if (!CharacterMesh)
        return nullptr;

    UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(GetOwner());

	//장신구 데이터(Slot, Mesh, Loc, Roc 등)를 생성한 메쉬 컴포넌트에 하나씩 적용
    MeshComponent->RegisterComponent();

    MeshComponent->SetStaticMesh(Data.Mesh);

    if (Data.Material)
    {
        MeshComponent->SetMaterial(0, Data.Material);
    }

    MeshComponent->SetRelativeLocation(Data.RelativeLocation);
    MeshComponent->SetRelativeRotation(Data.RelativeRotation);
    MeshComponent->SetRelativeScale3D(Data.RelativeScale);

    MeshComponent->AttachToComponent(
        CharacterMesh,
        FAttachmentTransformRules::SnapToTargetIncludingScale,
        GetSocketName(Data.Slot)
    );

    return MeshComponent;
}

//Slot 이름을 대응되는 Skeletal Mesh Socket으로 변환
FName UAccessoryComponent::GetSocketName(EAccessorySlot Slot) const
{
    switch (Slot)
    {
    case EAccessorySlot::LeftFinger:
        return TEXT("RingSocket"); 
    
    case EAccessorySlot::Head:
        return TEXT("HeadSocket");

    case EAccessorySlot::Face:
        return TEXT("FaceSocket");

    case EAccessorySlot::RightFinger:
        return TEXT("Finger_R");    

    case EAccessorySlot::Neck:
        return TEXT("NeckSocket");
    }

    return NAME_None;
}

USkeletalMeshComponent* UAccessoryComponent::GetCharacterMesh() const
{
    APlayerCharacter* PCh = Cast<APlayerCharacter>(GetOwner());

    if (!PCh)
        return nullptr;

    return PCh->GetMesh();
}
