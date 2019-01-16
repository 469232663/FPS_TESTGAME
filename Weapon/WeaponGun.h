// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "WeaponGun.generated.h"

class UDamageType;
class UCameraShake;
class UMaterialInterface;

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TEnumAsByte< EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceTo;

};

UCLASS()
class FPS_TESTGAME_API AWeaponGun : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponGun();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent * WeaponSkletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "WeponeBasicValue")
	FName CurrentMuzzleName; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
	FName ShellEjectionName; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
	FTransform CurrentMuzzleTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
		TSubclassOf<AActor> BulletActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
		TSubclassOf< UDamageType > DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")	//����������������
		float AttackLinearVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
		FVector2D RandomRecoilPith;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
		FVector2D RandomRecoilYaw;
	////////////////////////////////////////////////////������Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem * FireParticle;		//������Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem * FireMuzzleSmokeParticle;		//��������Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem * FireShellEjectionParticle;		//���𵯿���Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem * DefaultImpactEffect;				//Ĭ�ϻ�����Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		TArray<UParticleSystem *> ImpactParticleArray;		//������Ч����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSound")
		USoundCue * FireSound;		//������Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
		TArray<UMaterialInterface *> ImpactDecalArray;	//���е�����������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
		UMaterialInterface * DefaultImpactDecal;		//���е�������
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponFireShake")
		TSubclassOf<UCameraShake> FireCameraShake;

	////////////////////////////////////////////////////��������
	float  LastFireTime;   //��¼֮ǰ�����ʱ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
	float  RateOfFire;   //ÿ���ӵĿ�������

	float  TimeBetweenShots;  //��ȡ������ʱ����

	FTimerHandle  TimerHandle_WeaponFireTimeHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContinuousFire")
	bool  bContinuousFire;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletFireValue")
	FVector GunMuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletFireValue")
	FRotator BulletSpawnRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletFireValue")
	FVector BulletSpawnLocation;


public:
	UFUNCTION(BlueprintCallable)
		UParticleSystem * GetImpactParticle(EPhysicalSurface  SurfaceType);

	void PlayImpactEffects(EPhysicalSurface  SurfaceType,FVector ImpactPoint);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerOnAttack();

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
		FHitScanTrace HitScanTrace;

	UFUNCTION()
		void OnRep_HitScanTrace();


private:
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnAttack()override;
	virtual void OffAttack()override;

		virtual void PlayWeaponParticle() override;		//������Ч�����ڴ�

		virtual bool Fire_Int_Implementation(bool isFire, float Time) override;

		virtual void SetCurrentMeshCollision(bool bCollision) override;
};
