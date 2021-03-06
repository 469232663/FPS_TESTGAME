// This code was written by 康子秋

#include "RestoreHealth.h"
#include "Components/StaticMeshComponent.h"
#include"Components/SphereComponent.h"
#include "Components/HealthComponent.h"
#include "PlayerCharacter/PlayerCharacterBase.h"
#include "Kismet/GameplayStatics.h"

ARestoreHealth::ARestoreHealth()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComp);
	WeaponHitSphere->SetupAttachment(MeshComp);

	HealthAmount = 20.0f;

}
void ARestoreHealth::BeginPlay()
{
	Super::BeginPlay();
	SetCurrentMeshCollision(true);

}
void ARestoreHealth::BeginHit(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	APlayerCharacterBase * PlayerCharacter = Cast<APlayerCharacterBase>(OtherActor);
	if (PlayerCharacter)
	{
		if (PlayerCharacter->HealthComp->Heal(HealthAmount))
		{
			if (DestroySound)
			{
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DestroySound, GetActorLocation());
			}
			Destroy();
		}
	}
}

void ARestoreHealth::SetCurrentMeshCollision(bool bCollision)
{
	if (bCollision)
	{
		MeshComp->SetSimulatePhysics(true);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{

		MeshComp->SetSimulatePhysics(false);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
