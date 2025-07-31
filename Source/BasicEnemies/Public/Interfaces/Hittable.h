// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hittable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHittable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BASICENEMIES_API IHittable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Hit(const FVector& ImpactPoint) = 0;
};
