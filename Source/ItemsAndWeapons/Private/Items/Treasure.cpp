// Lawliet Studio


#include "Items/Treasure.h"

#include "Interfaces/MeleeActor.h"
#include "Kismet/GameplayStatics.h"


ATreasure::ATreasure()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATreasure::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const IMeleeActor* MeleeActor = Cast<IMeleeActor>(OtherActor))
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		Destroy();
	}
}