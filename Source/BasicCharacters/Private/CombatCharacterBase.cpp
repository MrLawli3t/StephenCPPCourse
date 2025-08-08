// Lawliet Studio


#include "CombatCharacterBase.h"

#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"


ACombatCharacterBase::ACombatCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
}

void ACombatCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	OnTakePointDamage.AddDynamic(this, &ACombatCharacterBase::OnHit);
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
	PlayHitReactMontage(GetHitReactSection(HitLocation));

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
}

