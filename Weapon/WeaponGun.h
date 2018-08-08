// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "WeaponGun.generated.h"

class UDamageType;
class UCameraShake;
/**
 * 
 */
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem * FireParticle;		//������Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem * FireMuzzleSmokeParticle;		//��������Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem * FireShellEjectionParticle;		//���𵯿���Ч
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSound")
		USoundCue * FireSound;		//������Ч
	UPROPERTY(EditDefaultsOnly, Category = "WeaponFireShake")
		TSubclassOf<UCameraShake> FireCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletFireValue")
	FVector GunMuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletFireValue")
	FRotator BulletSpawnRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletFireValue")
	FVector BulletSpawnLocation;
private:
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual bool OnAttack()override;
	virtual bool OffAttack()override;

		virtual void PlayWeaponParticle() override;
	
};
