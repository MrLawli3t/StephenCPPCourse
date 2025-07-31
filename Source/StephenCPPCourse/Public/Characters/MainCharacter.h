// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterTypes.h"
#include "Interfaces/ItemInteractor.h"
#include "Items/Weapons/Weapon.h"
#include "MainCharacter.generated.h"

class AWeapon;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionInstance;
class UInputAction;
class UInputMappingContext;
class AItem;

UCLASS()
class STEPHENCPPCOURSE_API AMainCharacter : public ACharacter, public IItemInteractor
{
	GENERATED_BODY()

public:
	AMainCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void StartAttackTrace();

	UFUNCTION(BlueprintCallable)
	void StopAttackTrace();

protected:
	virtual void BeginPlay() override;

	virtual void Jump() override;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> TurnAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AttackAction;
	
private:
	
	void Move(const FInputActionInstance& InputActionInstance);
	void Turn(const FInputActionInstance& InputActionInstance);
	void ToggleEquipped(const FInputActionInstance& InputActionInstance);
	void Attack(const FInputActionInstance& InputActionInstance);

	void PlayAttackMontage();
	void PlayEquipDisarmMontage(const FName& SectionName);

	bool CanDisarm() const;
	bool CanArm() const;
	bool CanAttack() const;

	UFUNCTION(BlueprintCallable, meta=(AllowPrivateAccess=true))
	void AttackEnd();

	void AttackHit(FHitResult HitResult);

	UFUNCTION(BlueprintCallable, meta=(AllowPrivateAccess=true))
	void Arm();

	UFUNCTION(BlueprintCallable, meta=(AllowPrivateAccess=true))
	void Disarm();

	UFUNCTION()
	void EquipEnd(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleInstanceOnly, Category="Item")
	TObjectPtr<AItem> OverlappingItem;

	UPROPERTY(VisibleInstanceOnly, Category="Item")
	TObjectPtr<AWeapon> EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	TObjectPtr<UAnimMontage> EquipDisarmMontage;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	EActionState ActionState = EActionState::EAS_Unoccupied;

public:
	FORCEINLINE virtual void SetOverlappingItem(AItem* Item) override {OverlappingItem = Item;};
	FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;};
};
