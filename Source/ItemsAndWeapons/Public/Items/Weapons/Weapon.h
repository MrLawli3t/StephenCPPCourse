// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;
class IWeaponInteractor;

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnAttackHitSignature, FHitResult HitResult);

UCLASS()
class ITEMSANDWEAPONS_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	
	void Equip(USkeletalMeshComponent* Mesh, FName SocketName, AActor* NewOwner, APawn* NewInstigator);
	void StopAttackTrace();
	void AttackTrace();

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& Location);
private:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Attack")
	TObjectPtr<UBoxComponent> TraceBox;

	FVector PreviousTraceLocation = FVector::ZeroVector;
	
	TSet<AActor*> HitActors;

	UPROPERTY(EditAnywhere, Category="Attack")
	float BaseDamage = 10.f;

public:
	FORCEINLINE UAnimMontage* GetAttackMontage() const {return AttackMontage;};
};
