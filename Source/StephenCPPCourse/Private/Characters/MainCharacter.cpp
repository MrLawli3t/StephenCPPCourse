// Lawliet Studio


#include "Characters/MainCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Hittable.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMainCharacter::EquipEnd);
	}
	
}

void AMainCharacter::Jump()
{
	if (ActionState == EActionState::EAS_Equipping) return;
	Super::Jump();
}

void AMainCharacter::Move(const FInputActionInstance& InputActionInstance)
{
	if (ActionState == EActionState::EAS_Attacking) return;
	if (ActionState == EActionState::EAS_Equipping) return;
	
	const FVector2D InputVector = InputActionInstance.GetValue().Get<FVector2D>();
	if ((Controller != nullptr) && (InputVector != FVector2D::ZeroVector))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FRotationMatrix ControlRotationMatrix = FRotationMatrix(YawRotation);
		const FVector ControlForward = ControlRotationMatrix.GetUnitAxis(EAxis::X);
		const FVector ControlRight = ControlRotationMatrix.GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ControlForward*InputVector.X + ControlRight*InputVector.Y);
	}
}

void AMainCharacter::Turn(const FInputActionInstance& InputActionInstance)
{
	const FVector2D InputVector = InputActionInstance.GetValue().Get<FVector2D>();
	if ((Controller != nullptr) && (InputVector != FVector2D::ZeroVector))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void AMainCharacter::ToggleEquipped(const FInputActionInstance& InputActionInstance)
{
	if (ActionState == EActionState::EAS_Attacking) return;

	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		OverlappingWeapon->Equip(GetMesh(), FName("hand_r_socket"));
		EquippedWeapon = OverlappingWeapon;
		EquippedWeapon->OnAttackHit.BindUObject(this, &AMainCharacter::AttackHit);
		CharacterState = ECharacterState::ECS_EquippedOneHanded;
	} else
	{
		if (CanArm())
		{
			PlayEquipDisarmMontage(FName("Equip"));
		} else if (CanDisarm())
		{
			PlayEquipDisarmMontage(FName("Disarm"));
		}
	}
	
}

void AMainCharacter::Attack(const FInputActionInstance& InputActionInstance)
{
	if (CanAttack())
	{
		EquippedWeapon->StartAttackTrace();
		PlayAttackMontage();
	}
}

void AMainCharacter::PlayAttackMontage()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (UAnimMontage* AttackMontage = EquippedWeapon->GetAttackMontage())
		{
			ActionState = EActionState::EAS_Attacking;
			AnimInstance->Montage_Play(AttackMontage);
			const unsigned short Section = FMath::RandRange(1,3);
			AnimInstance->Montage_JumpToSection(FName("Attack" + FString::FromInt(Section)), AttackMontage);
		}
	}
}

void AMainCharacter::PlayEquipDisarmMontage(const FName& SectionName)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (EquipDisarmMontage)
		{
			ActionState = EActionState::EAS_Equipping;
			AnimInstance->Montage_Play(EquipDisarmMontage);
			AnimInstance->Montage_JumpToSection(SectionName, EquipDisarmMontage);
		}
	}
}

bool AMainCharacter::CanDisarm() const
{
	return CharacterState == ECharacterState::ECS_EquippedOneHanded && EquippedWeapon;
}

bool AMainCharacter::CanArm() const
{
	return CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

bool AMainCharacter::CanAttack() const
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped && EquippedWeapon;
}

void AMainCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
	if (EquippedWeapon)
	{
		EquippedWeapon->StopAttackTrace();
	}
}

void AMainCharacter::AttackHit(FHitResult HitResult)
{
	if (HitResult.GetActor())
	{
		if (IHittable* Hittable = Cast<IHittable>(HitResult.GetActor()))
		{
			Hittable->Hit(HitResult.ImpactPoint);
		}
	}
}

void AMainCharacter::Arm()
{
	if (!EquippedWeapon) return;

	CharacterState = ECharacterState::ECS_EquippedOneHanded;
	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), FName("hand_r_socket"));
}

void AMainCharacter::Disarm()
{
	if (!EquippedWeapon) return;
	
	CharacterState = ECharacterState::ECS_Unequipped;
	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), FName("weapon_back_socket"));
}

void AMainCharacter::EquipEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == EquipDisarmMontage)
	{
		ActionState = EActionState::EAS_Unoccupied;
	}
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		Input->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AMainCharacter::Turn);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jump);
		Input->BindAction(EquipAction, ETriggerEvent::Started, this, &AMainCharacter::ToggleEquipped);
		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &AMainCharacter::Attack);
	}
}

void AMainCharacter::StartAttackTrace()
{
	if (EquippedWeapon) EquippedWeapon->StartAttackTrace();
}

void AMainCharacter::StopAttackTrace()
{
	if (EquippedWeapon) EquippedWeapon->StopAttackTrace();
}
