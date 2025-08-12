// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASICCHARACTERS_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth = 100.f;

public:
	FORCEINLINE void ReceiveDamage(const float Damage);
	FORCEINLINE float GetHealthPercent() const {return Health / MaxHealth;}
	FORCEINLINE bool IsAlive() const {return Health > 0.f;}
};
