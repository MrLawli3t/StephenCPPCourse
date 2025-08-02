// Lawliet Studio


#include "Characters/MainCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/MeleeSystemComponent.h"
#include "Enums/MeleeStates.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/Weapons/Weapon.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	MeleeSystemComponent = CreateDefaultSubobject<UMeleeSystemComponent>(TEXT("MeleeSystemComponent"));
	MeleeSystemComponent->SetAutoActivate(true);
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainCharacter::Jump()
{
	if (MeleeSystemComponent->GetActionState() == EActionState::EAS_Equipping) return;
	Super::Jump();
}

void AMainCharacter::Move(const FInputActionInstance& InputActionInstance)
{
	if (MeleeSystemComponent->GetActionState() == EActionState::EAS_Attacking) return;
	if (MeleeSystemComponent->GetActionState() == EActionState::EAS_Equipping) return;
	
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

void AMainCharacter::OnToggleEquipped(const FInputActionInstance& InputActionInstance)
{
	MeleeSystemComponent->ToggleEquipped();
}

void AMainCharacter::OnAttack(const FInputActionInstance& InputActionInstance)
{
	MeleeSystemComponent->Attack();
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
		Input->BindAction(EquipAction, ETriggerEvent::Started, this, &AMainCharacter::OnToggleEquipped);
		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &AMainCharacter::OnAttack);
	}
}

void AMainCharacter::PlayAttackMontage()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		if (UAnimMontage* AttackMontage = MeleeSystemComponent->GetEquippedWeapon()->GetAttackMontage())
		{
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
			AnimInstance->Montage_Play(EquipDisarmMontage);
			AnimInstance->Montage_JumpToSection(SectionName, EquipDisarmMontage);
		}
	}
}