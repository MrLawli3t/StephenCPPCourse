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

	virtual void Tick(float DeltaSeconds) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category="Sounds")
	TObjectPtr<USoundBase> PickupSound;

	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	FRotator TreasureRotation = FRotator(0.f, 45.f, 0.f);
};
