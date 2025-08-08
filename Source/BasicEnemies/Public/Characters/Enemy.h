// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "CombatCharacterBase.h"
#include "Enemy.generated.h"

class UAnimMontage;

UCLASS()
class BASICENEMIES_API AEnemy : public ACombatCharacterBase
{
	GENERATED_BODY()

public:
	AEnemy();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	virtual void BeginPlay() override;

private:

};
