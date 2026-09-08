// Fill out your copyright notice in the Description page of Project Settings.


#include "Skin/AccessoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DataTable.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/Character.h"

UAccessoryComponent::UAccessoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAccessoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAccessoryComponent::Equip(FName AccessoryRow)
{
    if (!AccessoryTable)
        return;

    const FAccessoryData* Data =
        AccessoryTable->FindRow<FAccessoryData>(AccessoryRow, TEXT(""));

    if (!Data)
        return;

    Unequip(Data->Slot);

    UStaticMeshComponent* MeshComponent = CreateAccessory(*Data);

    FEquippedAccessory Equipped;
    Equipped.AccessoryRow = AccessoryRow;
    Equipped.MeshComponent = MeshComponent;

    EquippedAccessories.Add(Data->Slot, Equipped);
}

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

UStaticMeshComponent* UAccessoryComponent::CreateAccessory(const FAccessoryData& Data)
{
    USkeletalMeshComponent* CharacterMesh = GetCharacterMesh();

    if (!CharacterMesh)
        return nullptr;

    UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(GetOwner());

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

USkeletalMeshComponent* UAccessoryComponent::GetCharacterMesh() const
{
    APlayerCharacter* PCh = Cast<APlayerCharacter>(GetOwner());

    if (!PCh)
        return nullptr;

    return PCh->GetMesh();
}

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
