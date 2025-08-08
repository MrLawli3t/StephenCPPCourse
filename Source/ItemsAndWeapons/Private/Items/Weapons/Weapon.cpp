// Lawliet Studio


#include "Items/Weapons/Weapon.h"

#include "KismetTraceUtils.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{	
	TraceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TraceBox"));
	TraceBox->SetupAttachment(GetRootComponent());

	HitActors.Reserve(10);
}

void AWeapon::Equip(USkeletalMeshComponent* Mesh, FName SocketName)
{
	SetItemState(EItemState::EIS_Equipped);
	GetItemMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetSphereComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EmberParticles->Deactivate();
	
	SetActorTickEnabled(false);
	AttachToComponent(Mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
}

void AWeapon::StopAttackTrace()
{
	HitActors.Empty();
	PreviousTraceLocation = FVector::ZeroVector;
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	
}

void AWeapon::AttackTrace()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(WeaponAttackTrace);

	const FVector CurrentTraceLocation = TraceBox->GetComponentLocation();

	if (PreviousTraceLocation.IsZero()) PreviousTraceLocation = CurrentTraceLocation;

	TArray<FHitResult> HitResults;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(WeaponTrace), false);
	QueryParams.AddIgnoredActors(TArray<AActor*>({this, GetAttachParentActor()}));

	DrawDebugLine(GetWorld(), PreviousTraceLocation, CurrentTraceLocation, FColor(255, 0, 0, 100), false, 1.f, 0, 50.f);
	if (GetWorld()->SweepMultiByChannel(
		HitResults,
		PreviousTraceLocation,
		CurrentTraceLocation,
		TraceBox->GetComponentQuat(),
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeBox(TraceBox->GetScaledBoxExtent()),
		QueryParams
	))
	{
		for (const FHitResult HitResult : HitResults)
		{
			const AActor* HitActor = HitResult.GetActor();
			if (HitActor && !HitActors.Contains(HitActor))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitActor->GetName());
				OnAttackHit.ExecuteIfBound(HitResult);
				HitActors.Add(HitResult.GetActor());
				CreateFields(HitResult.ImpactPoint);
			}
		}
	}

	PreviousTraceLocation = CurrentTraceLocation;
}
