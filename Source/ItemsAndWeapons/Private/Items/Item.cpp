// Lawliet Studio

#include "Items/Item.h"

#include "Components/MeleeSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/MeleeActor.h"
#include "NiagaraComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	SetRootComponent(ItemMesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());

	EmberParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EmberParticles"));
	EmberParticles->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const IMeleeActor* MeleeActor = Cast<IMeleeActor>(OtherActor))
	{
		MeleeActor->GetMeleeSystemComponent()->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (const IMeleeActor* MeleeActor = Cast<IMeleeActor>(OtherActor))
	{
		MeleeActor->GetMeleeSystemComponent()->SetOverlappingItem(nullptr);
	}
}

// Called every frame
void AItem::Tick(const float DeltaTime)
{
	RunningTime += DeltaTime;
	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f,0.f,Amplitude * FMath::Sin(RunningTime*Frequency)));
	}
}
