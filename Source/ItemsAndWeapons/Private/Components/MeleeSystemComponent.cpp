// Lawliet Studio


#include "Components/MeleeSystemComponent.h"

#include "Items/Weapons/Weapon.h"


UMeleeSystemComponent::UMeleeSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UMeleeSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPawn = Cast<APawn>(GetOwner());
	
	if (!OwningPawn)
	{
		UE_LOG(LogTemp, Fatal, TEXT("UMeleeSystemComponent can only be used on Pawns!"));
	}
}

void UMeleeSystemComponent::ToggleEquipped()
{
	if (ActionState == EActionState::EAS_Attacking) return;

	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		OnFirstEquip.ExecuteIfBound(OverlappingWeapon);
		
		EquippedWeapon = OverlappingWeapon;
		EquippedWeapon->OnAttackHit.BindUObject(this, &UMeleeSystemComponent::AttackHit);
		EquipState = EEquipState::ECS_EquippedOneHanded;
	} else
	{
		if (CanArm())
		{
			ActionState = EActionState::EAS_Equipping;
			OnEquipUnequip.ExecuteIfBound(true);
		} else if (CanDisarm())
		{
			ActionState = EActionState::EAS_Equipping;
			OnEquipUnequip.ExecuteIfBound(false);
		}
	}
	
}

void UMeleeSystemComponent::Attack()
{
	if (CanAttack())
	{
		ActionState = EActionState::EAS_Attacking;
		OnAttack.ExecuteIfBound(ComboIndex);
		ComboIndex = (ComboIndex + 1) % 3;
	}
}

bool UMeleeSystemComponent::CanDisarm() const
{
	return EquipState == EEquipState::ECS_EquippedOneHanded && EquippedWeapon;
}

bool UMeleeSystemComponent::CanArm() const
{
	return EquipState == EEquipState::ECS_Unequipped && EquippedWeapon;
}

bool UMeleeSystemComponent::CanAttack() const
{
	return (ActionState == EActionState::EAS_Unoccupied || bCanContinueCombo) && EquipState != EEquipState::ECS_Unequipped && EquippedWeapon;
}

void UMeleeSystemComponent::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void UMeleeSystemComponent::AttackHit(FHitResult HitResult)
{
	if (HitResult.GetActor())
	{
		if (!OwningPawn) return;
		DamageEvent.HitInfo = HitResult;
		DamageEvent.Damage = 10.f;
		HitResult.GetActor()->TakeDamage(10.f, DamageEvent, OwningPawn->GetController(), OwningPawn);
	}
}

void UMeleeSystemComponent::Arm()
{
	if (!EquippedWeapon) return;

	EquipState = EEquipState::ECS_EquippedOneHanded;
	OnArmDisarm.ExecuteIfBound(true);
}

void UMeleeSystemComponent::Disarm()
{
	if (!EquippedWeapon) return;
	
	EquipState = EEquipState::ECS_Unequipped;
	OnArmDisarm.ExecuteIfBound(false);
}

void UMeleeSystemComponent::EquipEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}