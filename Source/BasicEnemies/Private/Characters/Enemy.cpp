// Lawliet Studio


#include "Characters/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
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

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	
	UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, FColor::Blue, 10.f, 1.f);
	
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

void AEnemy::Hit(const FVector& ImpactPoint)
{
	PlayHitReactMontage(GetHitReactSection(ImpactPoint));

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}
