// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPS_TESTGAMEGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_TESTGAME_API AFPS_TESTGAMEGameModeBase : public AGameMode
{
	GENERATED_BODY()

		AFPS_TESTGAMEGameModeBase();

public:
	virtual void BeginPlay();

	void CompleteMission(APawn * InstigatorPawn);

protected:

	FTimerHandle TimeHandle_BotSpawner;

	FTimerHandle TimeHandle_NextWaveStart;

	int32 NrOfBotsToSpawn;	//��ǰ��Ҫ���ɵĵ���

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "GameMode")
	float TimeBetweenWave;

protected:
	/* Called once on every new player that enters the gamemode ��ÿһ��������Ϸ��Ҷ�����һ��*/
	virtual FString InitNewPlayer(class APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;
	
	UFUNCTION(BlueprintImplementableEvent,Category = "GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	void CheckWaveState();

	void CheckAnyPlayer();

	void GameOver();

	virtual void Tick(float DeltaSeconds)override;

public:

	virtual void StartPlay()override;
};
