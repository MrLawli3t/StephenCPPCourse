// Lawliet Studio


#include "CombatCharacterBase.h"

#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/MeleeSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"


ACombatCharacterBase::ACombatCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

	MeleeSystemComponent = CreateDefaultSubobject<UMeleeSystemComponent>(TEXT("MeleeSystemComponent"));
	MeleeSystemComponent->SetAutoActivate(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ACombatCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	OnTakePointDamage.AddDynamic(this, &ACombatCharacterBase::OnHit);
	MeleeSystemComponent->OnPlayMontageSection.BindUObject(this, &ACombatCharacterBase::PlayMontageAtSection);
}

void ACombatCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACombatCharacterBase::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::NoCollision));
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

void ACombatCharacterBase::PlayMontageAtSection(UAnimMontage* Montage, const FName Section)
{
	if (!GetMesh()) return;
	
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (Montage)
		{
			AnimInstance->Montage_Play(Montage);
			AnimInstance->Montage_JumpToSection(Section, Montage);
		}
	}
}

void ACombatCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACombatCharacterBase::OnHit(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
	class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType,
	AActor* DamageCauser)
{
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

	if (AttributeComponent)
	{
		AttributeComponent->ReceiveDamage(Damage);
	}

	if (AttributeComponent && AttributeComponent->IsAlive())
	{
		PlayMontageAtSection(HitReactMontage, GetHitReactSection(HitLocation));
	} else
	{
		Die();
	}
}
