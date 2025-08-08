// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatCharacterBase.generated.h"

class UAttributeComponent;

UCLASS()
class BASICCHARACTERS_API ACombatCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatCharacterBase();

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
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> AttributeComponent;
};
