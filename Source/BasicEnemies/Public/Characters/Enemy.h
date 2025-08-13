// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "CombatCharacterBase.h"
#include "Enums/EnemyStates.h"
#include "Enums/CharacterTypes.h"
#include "Enemy.generated.h"

class AWeapon;
class UPawnSensingComponent;
class UAnimMontage;
class UHealthBarComponent;
class UWidgetComponent;

UCLASS()
class BASICENEMIES_API AEnemy : public ACombatCharacterBase
{
	GENERATED_BODY()

public:
	AEnemy();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnHit(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Die() override;
	
private:
	bool InTargetRange(const AActor* Target, double Radius) const;
	void MoveToTarget(AActor* Target);
	AActor* GetNextPatrolTarget() const;
	void PatrolTimerElapsed();
	
	
	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UPROPERTY(EditAnywhere)
	double ChasingRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 165.f;

	UPROPERTY(EditAnywhere)
	double ChaseSpeed = 400.f;
	
	UPROPERTY(EditAnywhere)
	double PatrolSpeed = 300.f;
	
	UPROPERTY()
	TObjectPtr<class AAIController> AIController;

	UPROPERTY(VisibleInstanceOnly, Category="AI")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditInstanceOnly, Category="AI")
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(VisibleAnywhere, Category="AI")
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidgetComponent;

	UPROPERTY(EditAnywhere, Category="Combat")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category="AI")
	double PatrolTargetAcceptanceRadius = 75.f;

	UPROPERTY(EditAnywhere, Category="AI")
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category="AI")
	float WaitMin = 5.f;
	
	UPROPERTY(EditAnywhere, Category="AI")
	float WaitMax = 10.f;

	float DespawnTime = 3.f;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EDeathPose> DeathPose = EDP_Alive;
};