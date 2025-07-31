// Lawliet Studio

#include "Items/Item.h"

#include "Components/SphereComponent.h"
#include "Interfaces/ItemInteractor.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	SetRootComponent(ItemMesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(ItemMesh);
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
	if (IItemInteractor* Interactor = Cast<IItemInteractor>(OtherActor))
	{
		Interactor->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (IItemInteractor* Interactor = Cast<IItemInteractor>(OtherActor))
	{
		Interactor->SetOverlappingItem(nullptr);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	RunningTime += DeltaTime;
	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f,0.f,Amplitude * FMath::Sin(RunningTime*Frequency)));
	}
}

template <typename T>
T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2; 
}