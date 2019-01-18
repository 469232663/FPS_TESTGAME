// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS_TESTGAMEGameModeBase.h"
#include "Weapon/SpawnWeapon/SpawnWeapon.h"
#include "TimerManager.h"
#include "Components/HealthComponent.h"
#include "FPS_TESTGAMEGameState.h"
#include "FPS_TESTGAMEPlayerState.h"
#include "Engine.h"

AFPS_TESTGAMEGameModeBase::AFPS_TESTGAMEGameModeBase()
{
	TimeBetweenWave = 5.0f;

	GameStateClass = AFPS_TESTGAMEGameState::StaticClass();
	PlayerStateClass = AFPS_TESTGAMEPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;

}
void AFPS_TESTGAMEGameModeBase::BeginPlay()
{
	Super::BeginPlay();


}

void AFPS_TESTGAMEGameModeBase::Tick(float DeltaSeconds)	//ÿ�����
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
}

void AFPS_TESTGAMEGameModeBase::CompleteMission(APawn * InstigatorPawn)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);
	}
	//UGameInstance* MyInstance = GetGameState<AFPS_TESTGAMEGameModeBase>()->GetGameInstance();
	//GetGameInstance();
	
}

FString AFPS_TESTGAMEGameModeBase::InitNewPlayer(APlayerController * NewPlayerController, const FUniqueNetIdRepl & UniqueId, const FString & Options, const FString & Portal)
{
	FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "NewPlayerJoin",true);

	return Result;
}


void AFPS_TESTGAMEGameModeBase::StartPlay()		//��Ϸ����ʱ����
{
	Super::StartPlay();

	PrepareForNextWave();
	CheckAnyPlayer();
}
void AFPS_TESTGAMEGameModeBase::PrepareForNextWave()
{

	GetWorldTimerManager().SetTimer(TimeHandle_NextWaveStart, this, &AFPS_TESTGAMEGameModeBase::StartWave, TimeBetweenWave, false);	//��ʱ���ò�����

	SetWaveState(EWaveState::WaitingToStart);

}
///////////////////////////////////////////////////////////////	//״̬���
void AFPS_TESTGAMEGameModeBase::CheckWaveState()
{
	bool bIsPrepareingForWave = GetWorldTimerManager().IsTimerActive(TimeHandle_NextWaveStart);

	if (NrOfBotsToSpawn > 0 || bIsPrepareingForWave)
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	for (FConstPawnIterator it = GetWorld()->GetPawnIterator();it ; ++it)
	{
		APawn * TestPawn = it->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}
		UHealthComponent * HealthComp = Cast<UHealthComponent>(TestPawn->GetComponentByClass(UHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		SetWaveState(EWaveState::WaveComplete);

		PrepareForNextWave();
	}

}

void AFPS_TESTGAMEGameModeBase::CheckAnyPlayer()	//�����������Ƿ���
{

	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		APlayerController * PC = it->Get();
		if (PC && PC->GetPawn())
		{
			APawn * MyPawn = PC->GetPawn();
			UHealthComponent * HealthComp = Cast<UHealthComponent>(MyPawn->GetComponentByClass(UHealthComponent::StaticClass()));

			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)	//��֤���ӵ��Ѫ�����
			{
				return;
			}
			
		}
	}

	//����Ҵ��


	GameOver();

}
///////////////////////////////////////////////////////////////
void AFPS_TESTGAMEGameModeBase::GameOver()
{
	EndWave();

	SetWaveState(EWaveState::GameOver);

	UE_LOG(LogTemp, Log, TEXT("GameOver!!!"));
}

void AFPS_TESTGAMEGameModeBase::StartWave()
{
	WaveCount++;

	NrOfBotsToSpawn = 15 * WaveCount;

	GetWorldTimerManager().SetTimer(TimeHandle_BotSpawner,this,&AFPS_TESTGAMEGameModeBase::SpawnBotTimerElapsed,1.0f ,true , 0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}
void AFPS_TESTGAMEGameModeBase::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NrOfBotsToSpawn--;

	if (NrOfBotsToSpawn <= 0)
	{
		EndWave();
	}


}
void AFPS_TESTGAMEGameModeBase::EndWave()
{

	GetWorldTimerManager().ClearTimer(TimeHandle_BotSpawner);

	SetWaveState(EWaveState::WaitingToComplete);
}
void AFPS_TESTGAMEGameModeBase::SetWaveState(EWaveState NewState)
{
	AFPS_TESTGAMEGameState * GS = GetGameState<AFPS_TESTGAMEGameState>();
	if (ensureAlways(GS))
	{
		GS->SetWaveState(NewState);
	}
}

void AFPS_TESTGAMEGameModeBase::RestarDeadPlayers()
{
	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		APlayerController * PC = it->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}

