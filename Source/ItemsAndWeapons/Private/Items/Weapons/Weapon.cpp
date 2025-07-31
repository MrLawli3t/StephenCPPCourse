// Lawliet Studio


#include "Items/Weapons/Weapon.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
	StartTrace = CreateDefaultSubobject<USceneComponent>(TEXT("StartTrace"));
	StartTrace->SetupAttachment(GetRootComponent());

	EndTrace = CreateDefaultSubobject<USceneComponent>(TEXT("EndTrace"));
	EndTrace->SetupAttachment(GetRootComponent());

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

void AWeapon::StartAttackTrace()
{
	if (AttackTraceTimer.IsValid()) GetWorldTimerManager().UnPauseTimer(AttackTraceTimer);
	else
	{
		GetWorldTimerManager().SetTimer(
		AttackTraceTimer,
		this,
		&AWeapon::AttackTrace,
		TraceDelay,
		true
	);
	}
}

void AWeapon::StopAttackTrace()
{
	GetWorldTimerManager().PauseTimer(AttackTraceTimer);
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
	const FVector Start = StartTrace->GetComponentLocation();
	const FVector End = EndTrace->GetComponentLocation();
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
		StartTrace->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1.f
	))
	{
		OnAttackHit.ExecuteIfBound(HitResult);
		HitActors.AddUnique(HitResult.GetActor());
	}
}
