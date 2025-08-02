// Lawliet Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MeleeActor.generated.h"

class UMeleeSystemComponent;
class AItem;

// This class does not need to be modified.
UINTERFACE()
class UMeleeActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ITEMSANDWEAPONS_API IMeleeActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UMeleeSystemComponent* GetMeleeSystemComponent() const = 0;
	virtual void PlayMontageAtSection(UAnimMontage* Montage, FName Section) = 0;
	virtual UAnimMontage* GetEquipDisarmMontage() const = 0;
	virtual USkeletalMeshComponent* IGetMesh() const = 0;
	virtual AController* IGetController() const = 0;
	virtual AActor* GetActor() = 0;
};