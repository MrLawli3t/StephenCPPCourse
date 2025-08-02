// Lawliet Studio

#pragma once

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Equipping UMETA(DisplayName = "Equipping"),
};

UENUM(BlueprintType)
enum class EEquipState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHanded UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHanded UMETA(DisplayName = "Equipped Two-Handed Weapon"),
};