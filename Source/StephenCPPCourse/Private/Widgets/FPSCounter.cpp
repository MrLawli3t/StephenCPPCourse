// Lawliet Studio


#include "Widgets/FPSCounter.h"

void UFPSCounter::NativeConstruct()
{
	Super::NativeConstruct();
	
	FPSHistory.Reserve(100);
}

void UFPSCounter::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FPSHistory.Add(1 / InDeltaTime);
}
