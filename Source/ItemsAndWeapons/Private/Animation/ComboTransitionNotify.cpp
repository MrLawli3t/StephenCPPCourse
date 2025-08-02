// Lawliet Studio


#include "Animation/ComboTransitionNotify.h"

#include "Components/MeleeSystemComponent.h"
#include "Interfaces/MeleeActor.h"

void UComboTransitionNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                         const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp || !MeshComp->GetOwner()) return;
	if (const IMeleeActor* Actor = Cast<IMeleeActor>(MeshComp->GetOwner()))
	{
		Actor->GetMeleeSystemComponent()->SetCanContinueCombo(true);
	}
}

void UComboTransitionNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner()) return;
	if (const IMeleeActor* Actor = Cast<IMeleeActor>(MeshComp->GetOwner()))
	{
		Actor->GetMeleeSystemComponent()->SetCanContinueCombo(false);
	}
}
