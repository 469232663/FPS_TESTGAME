// This code was written by 康子秋

#pragma once

#include "CoreMinimal.h"
#include "AnimState/AnimStateBase.h"
#include "PawnAnimStateBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_TESTGAME_API UPawnAnimStateBase : public UAnimStateBase
{
	GENERATED_BODY()
public:
	UPawnAnimStateBase();

	virtual void PawnAnimStateTick(class APlayerCharacterBase & PlayerCharacter,float DelateTime);
	
	virtual void PawnAnimState(class APlayerCharacterBase & PlayerCharacter);

public:
};
//static UPawnAnimState_Idle Anim_Idle;