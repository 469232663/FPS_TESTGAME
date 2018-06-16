// Fill out your copyright notice in the Description page of Project Settings.

#include "PUBG_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
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
		Pitch = FRotator::NormalizeAxis(DeltaRotation.Pitch) * 1.5f;

		//�����Ծ
		IsJump = CurrentPlayerPawn->GetMovementComponent()->IsFalling();

		//����¶�
		IsCrouch = CurrentPlayerPawn->GetMovementComponent()->IsCrouching();

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
