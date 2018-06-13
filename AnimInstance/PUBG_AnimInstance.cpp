// Fill out your copyright notice in the Description page of Project Settings.

#include "PUBG_AnimInstance.h"

void UPUBG_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	APawn * Pawn = TryGetPawnOwner();
	if (Pawn)
	{
		FVector Velocity = Pawn->GetVelocity();
		FRotator ActorRotation = Pawn->GetActorRotation();	//�õ���ǰActorRotation
		FRotator ControlRotation = Pawn->GetControlRotation();	//�õ���ǰPawn��ControlRotation

		//�����Ͽռ�
		Speed = Velocity.Size();
		Direction = CalculateDirection(Velocity, Pawn->GetActorRotation());	//�����ٶ���������ת��֮��ļн�

		//������׼ƫ��
		FRotator DeltaRotation = ControlRotation - ActorRotation;
		Yaw = FRotator::NormalizeAxis(DeltaRotation.Yaw);
		Pitch = FRotator::NormalizeAxis(DeltaRotation.Pitch);

	}

}



