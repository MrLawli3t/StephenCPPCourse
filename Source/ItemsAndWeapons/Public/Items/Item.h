// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class ITEMSANDWEAPONS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ItemMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	
	float RunningTime = 0;
	float Amplitude = 0.5f;
	float Frequency = 1.5f;
	
	EItemState ItemState = EItemState::EIS_Hovering;

public:
	FORCEINLINE UStaticMeshComponent* GetItemMesh() const {return ItemMesh;};
	FORCEINLINE USphereComponent* GetSphereComponent() const {return SphereComponent;};
	
	FORCEINLINE void SetItemState(const EItemState State) {ItemState = State;};
};