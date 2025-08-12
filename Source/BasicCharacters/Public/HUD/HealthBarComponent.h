// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"


class UHealthBar;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BASICCHARACTERS_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:	
	void SetHealthPercent(float Percent);

private:
	UPROPERTY()
	TObjectPtr<UHealthBar> HealthBarWidget;
};
