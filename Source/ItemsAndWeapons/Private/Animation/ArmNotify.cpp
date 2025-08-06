// Lawliet Studio


#include "Animation/ArmNotify.h"

#include "Components/MeleeSystemComponent.h"
#include "Interfaces/MeleeActor.h"

void UArmNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner()) return;
	if (const IMeleeActor* Actor = Cast<IMeleeActor>(MeshComp->GetOwner()))
	{
		Actor->GetMeleeSystemComponent()->Arm();
	}
}
