// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Physics/Experimental/ChaosEventType.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class ITEMSANDWEAPONS_API ABreakableActor : public AActor
{
	GENERATED_BODY()

public:
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;

	UFUNCTION()
	void OnBreak(const FChaosBreakEvent& BreakEvent);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<USoundBase> BreakSound;

	bool bIsBroken = false;

public:
	FORCEINLINE void SetClusteringEnabled(const bool bEnabled);
};
