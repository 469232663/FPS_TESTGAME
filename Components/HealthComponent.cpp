
#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"	


UHealthComponent::UHealthComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

	DefaultHealth = 100.0f;

	SetIsReplicated(true);
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwnerRole() == ROLE_Authority)	//�����ǰλ���Ƿ�����  Only hook if we serve
	{
		AActor * MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);	//Bind the Actor for TakeAnyDamage event
		}
	}
	
	Health = DefaultHealth;
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHealthComponent::HandleTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);	//�㲥�¼�

	UE_LOG(LogTemp, Log, TEXT("Health Changed : %s"), *FString::SanitizeFloat(Health));
}

void UHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = Health - OldHealth;

	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

bool UHealthComponent::Heal(float HealthAmount)
{
	if (HealthAmount <= 0 || Health <= 0 || Health >= DefaultHealth)
	{
		return false;
	}

	Health = FMath::Clamp(Health + HealthAmount, 0.0f, DefaultHealth);

	OnHealthChanged.Broadcast(this, Health, -HealthAmount, nullptr, nullptr, nullptr);

	return true;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const	//��Ա����
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health); //COND_SkipOwner��ֹ�ͻ��˽��������Ƶ���������������ٴ�֪ͨ�˿ͻ��ˣ��˲����ص�����Ч�����Ѿ�ִ�й��� ���������֪ͨ����ִ�У�
}
