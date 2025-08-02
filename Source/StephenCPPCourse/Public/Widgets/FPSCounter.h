// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSCounter.generated.h"

/**
 * 
 */
UCLASS()
class STEPHENCPPCOURSE_API UFPSCounter : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	
	UPROPERTY(BlueprintReadOnly)
	TArray<int> FPSHistory = {};
};
