// Lawliet Studio


#include "Characters/WukongAnimInstance.h"
#include "Characters/MainCharacter.h"
#include "Components/MeleeSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UWukongAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	if (MainCharacter)
	{
		CharacterMovementComponent = MainCharacter->GetCharacterMovement();
	}
}

void UWukongAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (CharacterMovementComponent && MainCharacter && MainCharacter->GetMeleeSystemComponent())
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovementComponent->Velocity);
		bIsFalling = CharacterMovementComponent->IsFalling();
		EquipState = MainCharacter->GetMeleeSystemComponent()->GetEquipState();
	}
}
