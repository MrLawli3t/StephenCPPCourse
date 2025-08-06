// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Treasure.generated.h"

UCLASS()
class ITEMSANDWEAPONS_API ATreasure : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATreasure();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category="Sounds")
	TObjectPtr<USoundBase> PickupSound;
};
