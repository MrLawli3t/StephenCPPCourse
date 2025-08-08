// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/MeleeActor.h"
#include "MainCharacter.generated.h"

class AWeapon;
class UMeleeSystemComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionInstance;
class UInputAction;
class UInputMappingContext;

UCLASS()
class STEPHENCPPCOURSE_API AMainCharacter : public ACharacter, public IMeleeActor
{
	GENERATED_BODY()

public:
	AMainCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PlayMontageAtSection(UAnimMontage* Montage, FName Section);

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
	void OnToggleEquipped(const FInputActionInstance& InputActionInstance);
	void OnAttack(const FInputActionInstance& InputActionInstance);
	void ArmDisarm(const bool bDoArm);
	void StartArmDisarmAnim(const bool bDoEquip);
	void AttackAnim(const int32 AttackIndex);
	void FirstEquipAnim(AWeapon* OverlappingWeapon);

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	TObjectPtr<UAnimMontage> EquipDisarmMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMeleeSystemComponent> MeleeSystemComponent;

public:
	FORCEINLINE virtual UMeleeSystemComponent* GetMeleeSystemComponent() const override {return MeleeSystemComponent;}
};
