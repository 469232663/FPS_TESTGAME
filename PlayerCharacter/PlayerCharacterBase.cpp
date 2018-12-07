
#include "PlayerCharacterBase.h"
#include "FPS_TESTGAME.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapon/WeaponBomb.h"
#include "Weapon/WeaponGun.h"
#include "Weapon/WeaponBase.h"
#include "Components/InputComponent.h"
#include "GameMode/FPS_TESTGAMEGameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"
#include "AnimState/PawnAnimState/PawnAnimStateBase.h"
APlayerCharacterBase::APlayerCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	GunTrenchNum = 2;	//Ĭ������2��������λ
	GunTrenchArray.SetNum(GunTrenchNum);
	GunTrenchArray[0].SetTrenchName("Weapon_A");
	GunTrenchArray[1].SetTrenchName("Weapon_B");
	bDied = false;
	bAim = false;
	//CurrentStateEnum = PlayerStateEnum::Idle;	//��ɫ״̬ö��
	CurrentWeaponAnimStateEnum = PlayerWeaponAnimStateEnum::GunComplete;	//��������ö��
	CurrentHandWeaponState = CurrentHandWeaponStateEnum::Hand;	//��ǰ��������ö��
	MovementComp = GetCharacterMovement();

	Wepone_Hand_name = "HandGun_R";

	SprintSpeed = 600;
	RunSpeed = 450;
	WalkSpeed = 190;
	CrouchSpeed = 190;

	DefaultFOV = 90.0f;
	ZoomedFOV = 60.0f;
	ZoomInterSpeed = 0.5f;


	//PlayerMeshStatic = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMeshStatic"));
	//PlayerMeshStatic->SetupAttachment(GetRootComponent());

	//PawnAnimState = &Idleing;

	//PawnAnimState->PawnAnimState(*this);

	CameraBoomComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoomComp"));
	CameraBoomComp->SetupAttachment(GetRootComponent());
	CameraBoomComp->SocketOffset = FVector(0, 35, 0);
	CameraBoomComp->TargetOffset = FVector(0, 0, 55);
	CameraBoomComp->bUsePawnControlRotation = true;	//��������ɫ��ת
	CameraBoomComp->bEnableCameraRotationLag = true;

	ThrowWeaponScene = CreateDefaultSubobject<USceneComponent>(TEXT("ThrowWeaponScene"));
	ThrowWeaponScene->SetupAttachment(GetRootComponent());

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(CameraBoomComp);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);		//���������������ײ

	static ConstructorHelpers::FObjectFinder<UCurveFloat> FindTurnBackCurve(TEXT("/Game/PUBG_Assent/Animation/TurnBackCurve")); //����TurnBackCurve
		TurnBackCurve = FindTurnBackCurve.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> FindAimSpringCurve(TEXT("/Game/PUBG_Assent/Animation/AimSpringCurve")); //����AimSpringCurve
		AimSpringCurve = FindAimSpringCurve.Object;

		SetReplicates(true);
		SetReplicateMovement(true);
		GetMesh()->SetIsReplicated(true);
}

void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultWeaponClass)	//������������
	{
		//AddWeapon_Int_Implementation(GetWorld()->SpawnActor<AWeaponGun>(DefaultWeaponClass, FVector(0, 0, 0), FRotator(0, 0, 0)));

		//AddWeapon_Int_Implementation(GetWorld()->SpawnActor<AWeaponGun>(DefaultWeaponClass, FVector(0, 0, 0), FRotator(0, 0, 0)));

		//GunTrenchArray[0].Weapon->ClonWeapon(GetTransform());	//���Կ�¡����
	}

	if (TurnBackCurve)
	{
		FOnTimelineFloat TurnBackTimelineCallBack;
		FOnTimelineEventStatic TurnBackTimelineFinishedCallback;	//��TimeLine���������õĺ���
		 
		TurnBackTimelineCallBack.BindUFunction(this, TEXT("UpdateControllerRotation"));//��һ�����ú������� �ڶ���Ϊ���ú�����
		TurnBackTimelineFinishedCallback.BindLambda([this]() {bUseControllerRotationYaw = false; });	//������ִ��Lambda���ʽ ��bUseControllerRotationYaw�ָ�Ϊtrue

		TurnBackTimeLine.AddInterpFloat(TurnBackCurve, TurnBackTimelineCallBack);	
		TurnBackTimeLine.SetTimelineFinishedFunc(TurnBackTimelineFinishedCallback);	//����TimeLine����������TurnBackTimelineFinishedCallback

	}

	if (AimSpringCurve)
	{
		FOnTimelineFloatStatic AimSpringTimelineCallBack;

		AimSpringTimelineCallBack.BindUFunction(this, TEXT("UpdateSpringLength"));

		AimSpringTimeLine.AddInterpFloat(AimSpringCurve, AimSpringTimelineCallBack);

	}


	AFPS_TESTGAMEGameModeBase * GM = Cast<AFPS_TESTGAMEGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		CurrentGameModeBase = GM;
	}

	HealthComp->OnHealthChanged.AddDynamic(this, &APlayerCharacterBase::OnHealthChanged);	//��Ѫ����Ӧ�¼�

}

void APlayerCharacterBase::OnHealthChanged(UHealthComponent * HealthComponent, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Health <= 0.0f && !bDied)	//��鲢ִ����������
	{
		PlayDieSet();

		bDied = true;

	}
}
void APlayerCharacterBase::PlayDieSet_Implementation()
{
	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetachFromControllerPendingDestroy();
	//SetLifeSpan(5.0f);  //�����Զ�����ʱ��
	GetMesh()->SetSimulatePhysics(true);

	/*for (int32 index = 0; index < GunTrenchArray.Num(); index++)
	{
	if (GunTrenchArray[index].IsWeapon())
	{
	GunTrenchArray[index].GetWeapon()->SetCurrentMeshCollision(false);
	}
	}*/
}
bool APlayerCharacterBase::PlayDieSet_Validate()
{
	return true;
}

void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TurnBackTimeLine.TickTimeline(DeltaTime); //tick�а�TimeLine
	AimSpringTimeLine.TickTimeline(DeltaTime);

	if (bDied)
	{
		
	}

	if (CurrentHandWeapon)
	{
		bUseControllerRotationYaw = true;
	}
	else 
	{
		bUseControllerRotationYaw = false;
	}


}
void APlayerCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this,&APlayerCharacterBase::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacterBase::SprintPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacterBase::SprintReleased);

	PlayerInputComponent->BindAction("Freelook", IE_Pressed, this, &APlayerCharacterBase::FreelookPressed);
	PlayerInputComponent->BindAction("Freelook", IE_Released, this, &APlayerCharacterBase::FreelookReleased); 

	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &APlayerCharacterBase::WalkPressed);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &APlayerCharacterBase::WalkReleased);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacterBase::CrouchPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacterBase::CrouchReleased);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacterBase::AimPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacterBase::AimReleased);


	PlayerInputComponent->BindAction("Weapon_1", IE_Pressed, this, &APlayerCharacterBase::Weapon_1Pressed);
	PlayerInputComponent->BindAction("Weapon_2", IE_Pressed, this, &APlayerCharacterBase::Weapon_2Pressed);
	PlayerInputComponent->BindAction("Hand", IE_Pressed, this, &APlayerCharacterBase::HandPressed);
	PlayerInputComponent->BindAction("ThrowWeapon", IE_Pressed, this, &APlayerCharacterBase::ThrowWeapon);
}

FVector APlayerCharacterBase::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

FRotator APlayerCharacterBase::GetViewRotation() const
{
	//if (CameraComp)
	{
		//return CameraComp->GetComponentRotation();
	}
	return Super::GetViewRotation();
}

//////////////////////////////////////////////////////////////////////////TimeLine���ú���
void APlayerCharacterBase::UpdateControllerRotation(float Value)
{
	FRotator NewRotation = FMath::Lerp(CurrentContrtolRotation, TargetControlRotation, Value);
	Controller->SetControlRotation(NewRotation);


}

void APlayerCharacterBase::UpdateSpringLength(float Value)
{
	float TargetFOV = bAim ? ZoomedFOV : DefaultFOV;
	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, Value, ZoomInterSpeed);

	CameraComp->SetFieldOfView(NewFOV);

}
//////////////////////////////////////////////////////////////////////////�������
void APlayerCharacterBase::AttackOn()
{
	if (CurrentHandWeapon && CurrentWeaponAnimStateEnum == PlayerWeaponAnimStateEnum::GunComplete)
	{
		CurrentHandWeapon->Execute_Fire_Int(CurrentHandWeapon, true, 0.0f);
	}
	
	bUseControllerRotationYaw = true;
	
}

void APlayerCharacterBase::AttackOff()
{
	if (CurrentHandWeapon && CurrentWeaponAnimStateEnum == PlayerWeaponAnimStateEnum::GunComplete)
	{
		CurrentHandWeapon->Execute_Fire_Int(CurrentHandWeapon, false, 0.0f);
	}

	if (!bAim)
	{
		bUseControllerRotationYaw = false;
	}
}

//////////////////////////////////////////////////////////////////////////���߿���
void APlayerCharacterBase::SprintPressed()
{
	if (!bAim)	
	{
		MovementComp->MaxWalkSpeed = SprintSpeed;
	}
	else   
	{
		MovementComp->MaxWalkSpeed = RunSpeed;   //��׼״̬����������ٶ�
	}
}

void APlayerCharacterBase::SprintReleased()
{
	if (!bAim)
	{
		MovementComp->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		MovementComp->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacterBase::WalkPressed()
{
	MovementComp->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacterBase::WalkReleased()
{

	MovementComp->MaxWalkSpeed = RunSpeed;

}
void APlayerCharacterBase::CrouchPressed()
{

	Crouch(true);
	MovementComp->MaxWalkSpeed = CrouchSpeed;
}
void APlayerCharacterBase::CrouchReleased()
{

	UnCrouch();
	MovementComp->MaxWalkSpeed = RunSpeed;

}
//////////////////////////////////////////////////////////////////////////�ӽǿ���
void APlayerCharacterBase::FreelookPressed()
{
	TargetControlRotation = GetControlRotation();
	bUseControllerRotationYaw = false;
}

void APlayerCharacterBase::FreelookReleased()
{
	CurrentContrtolRotation = GetControlRotation();
	TurnBackTimeLine.PlayFromStart();	//����TimeLine

}
void APlayerCharacterBase::AimPressed()
{

	AimSpringTimeLine.PlayFromStart();
	bAim = true;

	if (CurrentHandWeaponState != CurrentHandWeaponStateEnum::Hand)		//������ڵ���������ȭͷ��������·�ٶ�
	{
		MovementComp->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacterBase::AimReleased()
{

	AimSpringTimeLine.PlayFromStart();	//���𲥷�AimSpringTimeLine

	bAim = false;
	
	MovementComp->MaxWalkSpeed = RunSpeed;
	
}

//////////////////////////////////////////////////////////////////////////��������
void APlayerCharacterBase::Weapon_1Pressed()
{

	TakeWeapon(CurrentHandWeaponStateEnum::Weapon_1);
	
}
void APlayerCharacterBase::Weapon_2Pressed()
{
	
	TakeWeapon(CurrentHandWeaponStateEnum::Weapon_2);
	
}

void APlayerCharacterBase::HandPressed()
{
	
	TakeWeapon(CurrentHandWeaponStateEnum::Hand);
	
}

void APlayerCharacterBase::ThrowWeapon()
{
	if (CurrentHandWeapon)
	{
		//������ײ������TagΪһ���ַ��������Ժ�ʶ��true��TraceParams.bTraceComplex��this��InIgnoreActor    
		FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = false;    //ʹ�ø���Collision�ж��������ж�������׼ȷ        
		TraceParams.bTraceComplex = true;    /* FHitResults������ܽ�� */

		FHitResult Hit(ForceInit);
		if (GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), ThrowWeaponScene->GetComponentLocation(), ECollisionChannel::ECC_Camera,TraceParams))
		{
			CurrentHandWeapon->ClonWeapon(FTransform(Hit.Location));
		}
		else
		{
			CurrentHandWeapon->ClonWeapon(FTransform(ThrowWeaponScene->GetComponentLocation()));
		}
		CurrentHandWeapon->Destroy(true);
		CurrentHandWeapon = nullptr;
		TakeWeapon(CurrentHandWeaponStateEnum::Hand);
	}

}

void APlayerCharacterBase::TakeWeapon_Implementation(CurrentHandWeaponStateEnum HandWeaponEnum)
{
	if (CurrentWeaponAnimStateEnum == PlayerWeaponAnimStateEnum::GunComplete)
	{
		switch (HandWeaponEnum)
		{
		case CurrentHandWeaponStateEnum::Weapon_1:
			if (GunTrenchArray[0].IsWeapon())
			{
				CurrentHandWeaponState = HandWeaponEnum;
				CurrentWeaponAnimStateEnum = PlayerWeaponAnimStateEnum::Take_Gun;
			}
			break;
		case CurrentHandWeaponStateEnum::Weapon_2:
			if (GunTrenchArray[1].IsWeapon())
			{
				CurrentHandWeaponState = HandWeaponEnum;
				CurrentWeaponAnimStateEnum = PlayerWeaponAnimStateEnum::Take_Gun;
			}
			break;
		case CurrentHandWeaponStateEnum::Hand:
			if (CurrentHandWeaponState != CurrentHandWeaponStateEnum::Hand)
			{
				CurrentHandWeaponState = HandWeaponEnum;
				CurrentWeaponAnimStateEnum = PlayerWeaponAnimStateEnum::Down_Gun;
			}
			break;
		default:
			break;
		}
	}

}
bool APlayerCharacterBase::TakeWeapon_Validate(CurrentHandWeaponStateEnum HandWeaponEnum)
{
	return true;
}

void APlayerCharacterBase::UpdateWeapon()
{
	switch (CurrentHandWeaponState)
	{
	case CurrentHandWeaponStateEnum::Weapon_1:
		GetGunWeapon(0);
		break;
	case CurrentHandWeaponStateEnum::Weapon_2:

		GetGunWeapon(1);
		break;
	case CurrentHandWeaponStateEnum::Hand:
	
		AddWeapon_Int_Implementation(CurrentHandWeapon);
		CurrentHandWeapon = nullptr;
		
		break;
	default:
		break;
	}
	CurrentWeaponAnimStateEnum = PlayerWeaponAnimStateEnum::GunComplete;
}


bool APlayerCharacterBase::AddWeapon_Int_Implementation(AWeaponBase * Gun)
{
	if (Gun == nullptr)
	{
		return false;
	}
	
	for (int32 index = 0; index < GunTrenchArray.Num(); index++)
	{
		if (!GunTrenchArray[index].IsWeapon())
		{
			Gun->Execute_Fire_Int(Gun, false, 0.0f);
			GunTrenchArray[index].SetWeapon(GetMesh(), Gun);
			Gun->SetOwner(this);
			return true;
		}
	}
	return false;
}

AWeaponBase * APlayerCharacterBase::GetGunWeapon(int32 TrenchID)
{
	if (CurrentHandWeapon)
	{
		AddWeapon_Int_Implementation(CurrentHandWeapon);
		CurrentHandWeapon = nullptr;
	}

	if(GunTrenchArray.IsValidIndex(TrenchID))
	CurrentHandWeapon = GunTrenchArray[TrenchID].GetWeapon();

	if (CurrentHandWeapon)
	{
		CurrentHandWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Wepone_Hand_name);
		return CurrentHandWeapon;
	}
	
	return nullptr;
}

void APlayerCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const	//��Ա����
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacterBase, CurrentHandWeapon);
	DOREPLIFETIME(APlayerCharacterBase, CurrentWeaponAnimStateEnum);
	DOREPLIFETIME(APlayerCharacterBase, CurrentHandWeaponState);
	DOREPLIFETIME(APlayerCharacterBase, bDied);
}
