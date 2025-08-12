// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "Enums/CharacterTypes.h"
#include "GameFramework/Character.h"
#include "CombatCharacterBase.generated.h"

enum class EDeathPose : uint8;
class UHealthBarComponent;
class UWidgetComponent;
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
	virtual void OnHit(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void PlayHitReactMontage(const FName& SectionName);
	FName GetHitReactSection(const FVector& ImpactPoint) const;

	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidgetComponent;

	float DespawnTime = 3.f;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

private:
	void Die();
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, Category="Sound")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category="VFX")
	TObjectPtr<UParticleSystem> HitParticles;
};
