// Lawliet Studio


#include "Characters/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	OnTakePointDamage.AddDynamic(this, &AEnemy::OnHit);
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
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

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FName AEnemy::GetHitReactSection(const FVector& ImpactPoint) const
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

void AEnemy::OnHit(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
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
