// Lawliet Studio


#include "Characters/Enemy.h"

#include "Components/CapsuleComponent.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
}

void AEnemy::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}
