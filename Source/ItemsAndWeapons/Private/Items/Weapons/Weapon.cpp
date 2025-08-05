// Lawliet Studio


#include "Items/Weapons/Weapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
	StartTracePos = CreateDefaultSubobject<USceneComponent>(TEXT("StartTracePos"));
	StartTracePos->SetupAttachment(GetRootComponent());
	
	EndTracePos = CreateDefaultSubobject<USceneComponent>(TEXT("EndTracePos"));
	EndTracePos->SetupAttachment(GetRootComponent());
	
	TraceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TraceBox"));
	TraceBox->SetupAttachment(GetRootComponent());

	HitActors.Reserve(8);
}

void AWeapon::Equip(USkeletalMeshComponent* Mesh, FName SocketName)
{
	SetItemState(EItemState::EIS_Equipped);
	GetItemMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetSphereComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SetActorTickEnabled(false);
	AttachToComponent(Mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
}

void AWeapon::StopAttackTrace()
{
	HitActors.Empty();
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

	const FVector Start = StartTracePos->GetComponentLocation();
	const FVector End = EndTracePos->GetComponentLocation();	
	const FVector HalfSize = TraceBox->GetScaledBoxExtent();
	
	TArray<AActor*> ActorsToIgnore = {Owner};

	for (AActor* HitActor : HitActors)
	{
		ActorsToIgnore.Add(HitActor);
	}
	
	FHitResult HitResult;
	
	if (UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		HalfSize,
		StartTracePos->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1.f
	))
	{
		OnAttackHit.ExecuteIfBound(HitResult);
		HitActors.AddUnique(HitResult.GetActor());
		CreateFields(HitResult.ImpactPoint);
	}
}
