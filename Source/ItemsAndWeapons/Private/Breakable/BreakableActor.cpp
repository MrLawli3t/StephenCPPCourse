// Lawliet Studio


#include "Breakable/BreakableActor.h"

#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	SetRootComponent(GeometryCollectionComponent);
	GeometryCollectionComponent->bNotifyBreaks = true;
}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	GeometryCollectionComponent->ApplyBreakingLinearVelocity(0, FVector(0,0,1000));
	GeometryCollectionComponent->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::OnBreak);
}

void ABreakableActor::OnBreak(const FChaosBreakEvent& BreakEvent)
{
	if (BreakSound == nullptr) UE_LOG(LogTemp, Error, TEXT("BreakSound is not set for %s"), *GetName());
	if (bIsBroken) return;
	
	UGameplayStatics::PlaySoundAtLocation(this, BreakSound, GetActorLocation());
	SetLifeSpan(5);
	bIsBroken = true;
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

