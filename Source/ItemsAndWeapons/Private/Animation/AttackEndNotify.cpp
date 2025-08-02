// Lawliet Studio


#include "Animation/AttackEndNotify.h"

#include "Components/MeleeSystemComponent.h"
#include "Interfaces/MeleeActor.h"
#include "Items/Weapons/Weapon.h"

void UAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner()) return;
	if (const IMeleeActor* Actor = Cast<IMeleeActor>(MeshComp->GetOwner()))
	{
		Actor->GetMeleeSystemComponent()->AttackEnd();
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			AnimInstance->Montage_Stop(0.4f, Actor->GetMeleeSystemComponent()->GetEquippedWeapon()->GetAttackMontage());
		}
	}
}
