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
	GeometryCollectionComponent->ApplyBreakingLinearVelocity(0, FVector(0,0,1000));
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

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	UGameplayStatics::PlaySoundAtLocation(this, BreakSound, GetActorLocation());
	GetWorld()->SpawnActor<ATreasure>(TreasureClass, GetActorLocation() + FVector(0, 0, 75), GetActorRotation());
	SetLifeSpan(5);
	bIsBroken = true;
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

