
#include "PUBG_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/WeaponBase.h"
#include "PlayerCharacter/PlayerCharacterBase.h"

void UPUBG_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CurrentPlayerPawn = Cast<APlayerCharacterBase>(TryGetPawnOwner());
	if (CurrentPlayerPawn)
	{
		FVector Velocity = CurrentPlayerPawn->GetVelocity();
		FRotator ActorRotation = CurrentPlayerPawn->GetActorRotation();	//�õ���ǰActorRotation
		FRotator ControlRotation = CurrentPlayerPawn->GetControlRotation();	//�õ���ǰPawn��ControlRotation

		//�����Ͽռ�
		Speed = Velocity.Size();
		Direction = CalculateDirection(Velocity, CurrentPlayerPawn->GetActorRotation());	//�����ٶ���������ת��֮��ļн�

		//������׼ƫ��
		FRotator DeltaRotation = ControlRotation - ActorRotation;
		Yaw = FRotator::NormalizeAxis(DeltaRotation.Yaw);
		Pitch = FRotator::NormalizeAxis(DeltaRotation.Pitch) * 2.0f;

		//�����Ծ
		IsJump = CurrentPlayerPawn->GetMovementComponent()->IsFalling();

		//����¶�
		IsCrouch = CurrentPlayerPawn->GetMovementComponent()->IsCrouching();

		IsAim = CurrentPlayerPawn->bAim;

		if (CurrentPlayerPawn->CurrentHandWeapon)
		{
			isWeaponAttackFire = CurrentPlayerPawn->CurrentHandWeapon->isAttackFire;
			isWeaponReloading = CurrentPlayerPawn->CurrentHandWeapon->isReloading;
		}

	}

}

void UPUBG_AnimInstance::AnimNotify_TakeWeaponGun(UAnimNotify * Notify)
{
	if (CurrentPlayerPawn)
	{
		CurrentPlayerPawn->UpdateWeapon();
	}
}

void UPUBG_AnimInstance::AnimNotify_DownWeaponGun(UAnimNotify * Notify)
{
	if (CurrentPlayerPawn)
	{
		CurrentPlayerPawn->UpdateWeapon();
	}
}
