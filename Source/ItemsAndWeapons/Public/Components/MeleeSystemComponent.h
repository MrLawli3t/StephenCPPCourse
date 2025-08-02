// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DamageEvents.h"
#include "Enums/MeleeStates.h"
#include "MeleeSystemComponent.generated.h"


class IMeleeActor;
class AWeapon;
class AItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITEMSANDWEAPONS_API UMeleeSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMeleeSystemComponent();
	
	void StartAttackTrace();
	void StopAttackTrace();
	void EquipEnd();
	void Arm();
	void Disarm();
	void AttackEnd();
	
	void ToggleEquipped();
	void Attack();
	
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
	
	IMeleeActor* OwningActor = nullptr;
	
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
	FORCEINLINE USkeletalMeshComponent* GetMesh() const;
};
