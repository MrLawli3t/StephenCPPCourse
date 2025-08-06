// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Physics/Experimental/ChaosEventType.h"
#include "BreakableActor.generated.h"

class UCapsuleComponent;
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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;

	UFUNCTION()
	void OnBreak(const FChaosBreakEvent& BreakEvent);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<USoundBase> BreakSound;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<class ATreasure> TreasureClass;
	
	bool bIsBroken = false;

public:
	FORCEINLINE void SetClusteringEnabled(const bool bEnabled);
};
