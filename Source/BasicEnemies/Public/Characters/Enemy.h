// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UAnimMontage;

UCLASS()
class BASICENEMIES_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnHit(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

protected:
	virtual void BeginPlay() override;

	void PlayHitReactMontage(const FName& SectionName);
	FName GetHitReactSection(const FVector& ImpactPoint) const;

private:
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Sound")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category="VFX")
	TObjectPtr<UParticleSystem> HitParticles;
public:
};
