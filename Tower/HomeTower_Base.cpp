// This code was written by 康子秋

#include "HomeTower_Base.h"
#include "Components/HealthComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AHomeTower_Base::AHomeTower_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &AHomeTower_Base::OnHealthChanged);	//绑定血量响应事件

	HomeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HomeMeshComp"));
	SetRootComponent(HomeMeshComp);
}

// Called when the game starts or when spawned
void AHomeTower_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHomeTower_Base::TowerDestory()
{
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorLocation().Rotation(), FVector(4, 4, 4));
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}

}

// Called every frame
void AHomeTower_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHomeTower_Base::OnHealthChanged(UHealthComponent * HealthComponent, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (DamageSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DamageSound, GetActorLocation());
	}
	
	if (Health <= 0.0f)
	{
		TowerDestory();
	}
}
