// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInteractor.generated.h"

class AItem;

// This class does not need to be modified.
UINTERFACE()
class UItemInteractor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ITEMSANDWEAPONS_API IItemInteractor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetOverlappingItem(AItem* Item) = 0;
	
};