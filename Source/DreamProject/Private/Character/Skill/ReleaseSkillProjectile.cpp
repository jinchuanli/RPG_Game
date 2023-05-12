// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/ReleaseSkillProjectile.h"

#include "Character/Skill/ReleaseSkill.h"
#include "Enemy/NormalEnemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Enemy/Boss.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AReleaseSkillProjectile::AReleaseSkillProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	Niagaracomponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagaracomponent"));
	Niagaracomponent->SetupAttachment(RootComponent);

	BoxCollsion = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollsion"));
	BoxCollsion->OnComponentBeginOverlap.AddDynamic(this,&AReleaseSkillProjectile::OnOverlap);
	BoxCollsion->SetupAttachment(Niagaracomponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void AReleaseSkillProjectile::BeginPlay()
{
	Super::BeginPlay();
	Skill = Cast<AReleaseSkill>(GetOwner());
	MissileEffect = Skill->MissileEffect;
	ImpactEffect = Skill->ImpactEffect;
	//ParticleSystem->SetTemplate(MissileEffect);
	Niagaracomponent->SetAsset(MissileEffect);

	//ProjectileMovementComponent->InitialSpeed = Skill->GetCurrentStage().MisiileSpeed;
	SetRotation();
	SetVelocity();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Destory,this,&AReleaseSkillProjectile::DestroySkill,1.f,false);

	//UE_LOG(LogTemp, Warning, TEXT("旋转方向 %s"),*GetActorRotation().ToString());
}

void AReleaseSkillProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANormalEnemy* TempEnemy = Cast<ANormalEnemy>(OtherActor);
	if(TempEnemy && OtherComp == TempEnemy->GetCapsuleComponent())
	{
		//UGameplayStatics::SpawnEmitterAtLocation(this,ImpactEffect,FVector(SweepResult.ImpactPoint),FRotator::ZeroRotator);
		if(ImpactEffect)
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ImpactEffect,FVector(SweepResult.ImpactPoint),FRotator::ZeroRotator);
		if(Sound)
			UGameplayStatics::PlaySoundAtLocation(this,Sound,OtherActor->GetActorLocation());
		IDamageAbleInterface* DamageAbleInterface = Cast<IDamageAbleInterface>(TempEnemy);
		if(DamageAbleInterface)
		{
			DamageAbleInterface->OnReceiveDamage(
				Skill->GetCurrentStage().Damage,
				Skill->GetCurrentStage().DamageType,
				Skill->GetSkillInfo().Element,
				Skill->GetCurrentStage().CriticalChance,
				Skill->GetPlayerReference(),
				Skill);
			Destroy();
		}
	}
	else
	{
		ABoss* BossRef = Cast<ABoss>(OtherActor);
		if(BossRef && OtherComp == BossRef->GetCapsuleComponent())
		{
			if(ImpactEffect)
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ImpactEffect,FVector(SweepResult.ImpactPoint),FRotator::ZeroRotator);
			if(Sound)
				UGameplayStatics::PlaySoundAtLocation(this,Sound,OtherActor->GetActorLocation());
			IDamageAbleInterface* DamageAbleInterface = Cast<IDamageAbleInterface>(BossRef);
			if(DamageAbleInterface)
			{
				DamageAbleInterface->OnReceiveDamage(
				Skill->GetCurrentStage().Damage,
				Skill->GetCurrentStage().DamageType,
				Skill->GetSkillInfo().Element,
				Skill->GetCurrentStage().CriticalChance,
				Skill->GetPlayerReference(),
				Skill);
				Destroy();
			}
		}
	}
}

// Called every frame
void AReleaseSkillProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AReleaseSkillProjectile::SetVelocity()
{
	//获得屏幕正中心的朝向
	FVector FVectorVelocity = GetWorld()->GetFirstPlayerController()->GetControlRotation().Vector().GetSafeNormal();
	FVectorVelocity *= Skill->GetCurrentStage().MisiileSpeed;
	ProjectileMovementComponent->Velocity = FVectorVelocity;
}

void AReleaseSkillProjectile::DestroySkill()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Destory);
	Destroy();
}

void AReleaseSkillProjectile::SetRotation()
{
	ARBaseCharacter* BaseCharacter = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	FRotator Rotator;
	FVector CollisionLocation;
	if(BaseCharacter)
	{
		Rotator = BaseCharacter->GetMesh()->GetSocketRotation("ProjectileSkill");
		//CollisionLocation = BaseCharacter->GetMesh()->GetSocketLocation("ProjectileSkill");
	}
	//BoxCollsion->SetWorldLocation(CollisionLocation);
	//BoxCollsion->SetWorldRotation(Rotator);
	Niagaracomponent->SetWorldRotation(Rotator);
}

