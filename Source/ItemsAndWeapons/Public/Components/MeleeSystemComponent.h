// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DamageEvents.h"
#include "Enums/MeleeStates.h"
#include "MeleeSystemComponent.generated.h"

class AWeapon;
class AItem;

DECLARE_DELEGATE_OneParam(FOnArmDisarmSignature, bool bDoArm);
DECLARE_DELEGATE_OneParam(FOnEquipUnequip, bool bDoEquip);
DECLARE_DELEGATE_OneParam(FOnAttackSignature, int32 AttackIndex);
DECLARE_DELEGATE_OneParam(FOnFirstEquipSingature, AWeapon* OverlappingWeapon);

UCLASS(ClassGroup=(Pawn), meta=(BlueprintSpawnableComponent))
class ITEMSANDWEAPONS_API UMeleeSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMeleeSystemComponent();
	
	void EquipEnd();
	void Arm();
	void Disarm();
	void AttackEnd();
	
	void ToggleEquipped();
	void Attack();
	
	// Necessary pointers checked before broadcast 
	FOnArmDisarmSignature OnArmDisarm;
	FOnEquipUnequip OnEquipUnequip;
	FOnAttackSignature OnAttack;
	FOnFirstEquipSingature OnFirstEquip;
	
protected:
	virtual void BeginPlay() override;

private:
	bool CanDisarm() const;
	bool CanArm() const;
	bool CanAttack() const;

	void AttackHit(FHitResult HitResult);

	UPROPERTY(VisibleInstanceOnly, Category="Item")
	TObjectPtr<AItem> OverlappingItem;
	
	UPROPERTY(VisibleInstanceOnly, Category="Item")
	TObjectPtr<AWeapon> EquippedWeapon;

	UPROPERTY()
	TObjectPtr<APawn> OwningPawn;
	
	FPointDamageEvent DamageEvent;

	int ComboIndex = 0;
	bool bCanContinueCombo = false;

	EActionState ActionState = EActionState::EAS_Unoccupied;
	EEquipState EquipState = EEquipState::ECS_Unequipped;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) {OverlappingItem = Item;}
	FORCEINLINE EActionState GetActionState() const {return ActionState;}
	FORCEINLINE EEquipState GetEquipState() const {return EquipState;}
	FORCEINLINE void SetCanContinueCombo(const bool CanContinue) {bCanContinueCombo = CanContinue;}
	FORCEINLINE AWeapon* GetEquippedWeapon() const {return EquippedWeapon;}
	FORCEINLINE AItem* GetOverlappingItem() const {return OverlappingItem;}
};
