// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Hittable.h"
#include "Enemy.generated.h"

class UAnimMontage;

UCLASS()
class BASICENEMIES_API AEnemy : public ACharacter, public IHittable
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Hit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	void PlayHitReactMontage(const FName& SectionName);
	FName GetHitReactSection(const FVector& ImpactPoint) const;

private:
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Sound")
	TObjectPtr<USoundBase> HitSound;
public:
};
