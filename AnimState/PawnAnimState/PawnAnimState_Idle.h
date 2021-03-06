// This code was written by 康子秋

#pragma once

#include "CoreMinimal.h"
#include "AnimState/PawnAnimState/PawnAnimStateBase.h"
#include "PawnAnimState_Idle.generated.h"

/**
 * 
 */
UCLASS()
class FPS_TESTGAME_API UPawnAnimState_Idle : public UPawnAnimStateBase
{
	GENERATED_BODY()
	
public:
	UPawnAnimState_Idle();

	virtual void PawnAnimStateTick(class APlayerCharacterBase & PlayerCharacter, float DelateTime);

	virtual void PawnAnimState(class APlayerCharacterBase & PlayerCharacter);
	
};
