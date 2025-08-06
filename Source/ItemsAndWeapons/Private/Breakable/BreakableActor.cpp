// Lawliet Studio


#include "Breakable/BreakableActor.h"

#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	SetRootComponent(GeometryCollectionComponent);
	GeometryCollectionComponent->bNotifyBreaks = true;
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(GeometryCollectionComponent);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	GeometryCollectionComponent->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::OnBreak);
}

void ABreakableActor::OnBreak(const FChaosBreakEvent& BreakEvent)
{
	if (BreakSound == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BreakSound is not set for %s"), *GetName());
		return;
	}
	if (bIsBroken) return;
	if (!GetWorld()) return;
	if (TreasureClasses.Num() == 0) return;

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	UGameplayStatics::PlaySoundAtLocation(this, BreakSound, GeometryCollectionComponent->GetComponentLocation());

	const TSubclassOf<ATreasure> TreasureClass = TreasureClasses[FMath::RandRange(0, TreasureClasses.Num() - 1)];
	GetWorld()->SpawnActor<ATreasure>(TreasureClass, GeometryCollectionComponent->GetComponentLocation() + FVector(0, 0, 25), FRotator::ZeroRotator);
	
	SetLifeSpan(3);
	bIsBroken = true;
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

