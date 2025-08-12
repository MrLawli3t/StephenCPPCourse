// Lawliet Studio


#include "CombatCharacterBase.h"

#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enums/CharacterTypes.h"
#include "GameFramework/CharacterMovementComponent.h"


ACombatCharacterBase::ACombatCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

	HealthBarWidgetComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ACombatCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	OnTakePointDamage.AddDynamic(this, &ACombatCharacterBase::OnHit);

	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(false);
	}
}

void ACombatCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACombatCharacterBase::Die()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (DeathMontage)
		{
			const int32 Section = FMath::RandRange(1, 2);
			AnimInstance->Montage_Play(DeathMontage);
			switch (Section)
			{
			case 1:
				AnimInstance->Montage_JumpToSection("Death1", DeathMontage);
				DeathPose = EDeathPose::EDP_Death1;
				break;
			case 2:
				AnimInstance->Montage_JumpToSection("Death2", DeathMontage);
				DeathPose = EDeathPose::EDP_Death2;
				break;
			default:
				break;
			}
		}
	}

	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(false);
	}

	GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::NoCollision));
	SetLifeSpan(DespawnTime);
}

void ACombatCharacterBase::PlayHitReactMontage(const FName& SectionName)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (HitReactMontage)
		{
			AnimInstance->Montage_Play(HitReactMontage);
			AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
		}
	}
}

FName ACombatCharacterBase::GetHitReactSection(const FVector& ImpactPoint) const
{
	FName Section = "FromBack";
	const FVector Forward = GetActorForwardVector();
	const FVector ToImpact = (ImpactPoint - GetActorLocation()).GetSafeNormal();

	float Theta = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Forward, ToImpact)));

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToImpact);
	
	//UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, FColor::Blue, 10.f, 1.f);
	
	if (CrossProduct.Z < 0.f) Theta *= -1.f;

	if (Theta >= -45.f && Theta <= 45.f)
	{
		Section = "FromFront";
	} else if (Theta > 45.f && Theta <= 135.f)
	{
		Section = "FromRight";
	} else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = "FromLeft";
	}

	return Section;
}

void ACombatCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACombatCharacterBase::OnHit(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
	class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
	AActor* DamageCauser)
{
	CombatTarget = InstigatedBy->GetPawn();
	
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(true);
	}
	
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, HitLocation);
	}
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			HitParticles,
			HitLocation
		);
	}

	if (AttributeComponent && HealthBarWidgetComponent)
	{
		AttributeComponent->ReceiveDamage(Damage);
		HealthBarWidgetComponent->SetHealthPercent(AttributeComponent->GetHealthPercent());
	}

	if (AttributeComponent && AttributeComponent->IsAlive())
	{
		PlayHitReactMontage(GetHitReactSection(HitLocation));
	} else
	{
		Die();
	}
}
