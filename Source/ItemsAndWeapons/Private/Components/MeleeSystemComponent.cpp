// Lawliet Studio


#include "Components/MeleeSystemComponent.h"

#include "Interfaces/MeleeActor.h"
#include "Items/Weapons/Weapon.h"


UMeleeSystemComponent::UMeleeSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UMeleeSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IMeleeActor* Actor = Cast<IMeleeActor>(GetOwner()))
	{
		OwningActor = Actor;
	}
	
}

void UMeleeSystemComponent::ToggleEquipped()
{
	if (ActionState == EActionState::EAS_Attacking) return;

	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		if (!GetMesh()) return;
		
		OverlappingWeapon->Equip(GetMesh(), FName("hand_r_socket"));
		EquippedWeapon = OverlappingWeapon;
		EquippedWeapon->OnAttackHit.BindUObject(this, &UMeleeSystemComponent::AttackHit);
		EquipState = EEquipState::ECS_EquippedOneHanded;
	} else
	{
		if (CanArm())
		{
			ActionState = EActionState::EAS_Equipping;
			OwningActor->PlayMontageAtSection(OwningActor->GetEquipDisarmMontage(), FName("Equip"));
		} else if (CanDisarm())
		{
			ActionState = EActionState::EAS_Equipping;
			OwningActor->PlayMontageAtSection(OwningActor->GetEquipDisarmMontage(), (FName("Disarm")));
		}
	}
	
}

void UMeleeSystemComponent::Attack()
{
	if (CanAttack())
	{
		EquippedWeapon->StartAttackTrace();
		ActionState = EActionState::EAS_Attacking;
		OwningActor->PlayMontageAtSection(EquippedWeapon->GetAttackMontage(), FName("Attack" + FString::FromInt(ComboIndex+1)));
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
		if (!OwningActor) return;
		DamageEvent.HitInfo = HitResult;
		DamageEvent.Damage = 10.f;
		HitResult.GetActor()->TakeDamage(10.f, DamageEvent, OwningActor->IGetController(), OwningActor->GetActor());
	}
}

USkeletalMeshComponent* UMeleeSystemComponent::GetMesh() const
{
	if (OwningActor && OwningActor->IGetMesh()) return OwningActor->IGetMesh();
	return nullptr;
}

void UMeleeSystemComponent::Arm()
{
	if (!EquippedWeapon) return;

	EquipState = EEquipState::ECS_EquippedOneHanded;
	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), FName("hand_r_socket"));
}

void UMeleeSystemComponent::Disarm()
{
	if (!EquippedWeapon) return;
	
	EquipState = EEquipState::ECS_Unequipped;
	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), FName("weapon_back_socket"));
}

void UMeleeSystemComponent::EquipEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void UMeleeSystemComponent::StartAttackTrace()
{
	if (EquippedWeapon) EquippedWeapon->StartAttackTrace();
}

void UMeleeSystemComponent::StopAttackTrace()
{
	if (EquippedWeapon) EquippedWeapon->StopAttackTrace();
}