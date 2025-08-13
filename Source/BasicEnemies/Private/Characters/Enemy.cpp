// Lawliet Studio


#include "Characters/Enemy.h"

#include "AIController.h"
#include "AITypes.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/MeleeSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	HealthBarWidgetComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SetPeripheralVisionAngle(60.f);
	PawnSensingComponent->SightRadius = ChasingRadius;
}

void AEnemy::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::OnHit(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
	class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
	AActor* DamageCauser)
{
	Super::OnHit(DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);

	CombatTarget = InstigatedBy->GetPawn();
	
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(true);
		HealthBarWidgetComponent->SetHealthPercent(AttributeComponent->GetHealthPercent());
	}
	
	CombatTarget = InstigatedBy->GetPawn();
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	MoveToTarget(CombatTarget);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	PatrolTarget = GetNextPatrolTarget();
	
	AIController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnPawnSeen);
	}

	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(false);
	}

	if (UWorld* World = GetWorld())
	{
		if (WeaponClass != nullptr)
		{
			AWeapon* Weapon = World->SpawnActor<AWeapon>(WeaponClass);
			Weapon->Equip(GetMesh(), FName("hand_r_socket"), this, this);
		}
	}
}

void AEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		if (!InTargetRange(CombatTarget, ChasingRadius))
        {
        	CombatTarget = nullptr;
        	if (HealthBarWidgetComponent)
        	{
        		HealthBarWidgetComponent->SetVisibility(false);
        	}
			EnemyState = EEnemyState::EES_Patrolling;
			GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
			MoveToTarget(PatrolTarget);
			UE_LOG(LogTemp, Warning, TEXT("Lost enemy target"));
        } else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
        {
	        EnemyState = EEnemyState::EES_Chasing;
        	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
        	MoveToTarget(CombatTarget);
        	UE_LOG(LogTemp, Warning, TEXT("Chasing enemy target"));
        } else if (InTargetRange(CombatTarget, AttackRadius))
        {
        	EnemyState = EEnemyState::EES_Attacking;
        	UE_LOG(LogTemp, Warning, TEXT("Attacking enemy target"));
        	MeleeSystemComponent->Attack();
        }
	} else
	{
		if (InTargetRange(PatrolTarget, PatrolRadius))
        {
			PatrolTarget = GetNextPatrolTarget();
        	const int32 WaitTime = FMath::RandRange(WaitMin, WaitMax);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Wait time: %i"), WaitTime));
        	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerElapsed, WaitTime);
        }
	}
}

void AEnemy::Die()
{
	Super::Die();

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (DeathMontage)
		{
			const int32 Section = FMath::RandRange(1, 2);
			const TEnumAsByte<EDeathPose> Pose(Section);

			if (Pose > EDeathPose::EDP_Death2) return;
			PlayMontageAtSection(DeathMontage, DeathMontage->GetSectionName(Section));
		}
	}

	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(false);
	}

	SetLifeSpan(DespawnTime);
}

bool AEnemy::InTargetRange(const AActor* Target, const double Radius) const
{
	if (!Target) return false;
	if (Radius <= 0.01f) return false;
	
	const double DistanceToTargetXY = FVector::DistXY(Target->GetActorLocation(), GetActorLocation());
	return DistanceToTargetXY < Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (AIController && Target)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(PatrolTargetAcceptanceRadius);
		AIController->MoveTo(MoveRequest);
	}
}

AActor* AEnemy::GetNextPatrolTarget() const
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.Add(Target);
		}
	}

	if (ValidTargets.Num() > 0)
	{
		const int32 Index = FMath::RandRange(0, ValidTargets.Num() - 1);
		return ValidTargets[Index];
	}
	
	return nullptr;
}

void AEnemy::PatrolTimerElapsed()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::OnPawnSeen(APawn* SeenPawn)
{
	if (EnemyState == EEnemyState::EES_Chasing) return;
	if (EnemyState == EEnemyState::EES_Attacking) return;
	if (SeenPawn->ActorHasTag(FName("EnemyTarget")))
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
		CombatTarget = SeenPawn;
		MoveToTarget(CombatTarget);
		UE_LOG(LogTemp, Warning, TEXT("Seen enemy target"));
	}
}
