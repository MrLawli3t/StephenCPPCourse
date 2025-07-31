// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

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
	
	void Equip(USkeletalMeshComponent* Mesh, FName SocketName);
	void StartAttackTrace();
	void StopAttackTrace();
	
	FOnAttackHitSignature OnAttackHit;
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
private:
	virtual void BeginPlay() override;

	void AttackTrace();
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(VisibleAnywhere, Category="Attack")
	TObjectPtr<USceneComponent> StartTrace;

	UPROPERTY(VisibleAnywhere, Category="Attack")
	TObjectPtr<USceneComponent> EndTrace;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	FVector HalfSize = FVector(5.f, 5.f, 5.f);

	FTimerHandle AttackTraceTimer;

	TArray<AActor*> HitActors;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float TraceDelay = 0.01f;

public:
	FORCEINLINE UAnimMontage* GetAttackMontage() const {return AttackMontage;};
};
