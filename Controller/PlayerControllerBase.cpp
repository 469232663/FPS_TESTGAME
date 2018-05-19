// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBase.h"
#include "PlayerCharacter/PlayerCharacterBase.h"
#include "Engine.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	PlayPawn = Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn());	//��ȡ����ǰPlayPawn
	
}

void APlayerControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}
