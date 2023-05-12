// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Boss_Skill.h"

#include "EngineUtils.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/RBaseCharacter.h"
#include "Character/Skill/BaseElement.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/Boss.h"

// Sets default values
ABoss_Skill::ABoss_Skill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this,&ABoss_Skill::OnOverlap);
	BoxCollision->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void ABoss_Skill::BeginPlay()
{
	Super::BeginPlay();
	GetBoss();
	NiagaraComponent->SetAsset(MissileEffect);
	SetHomingTarget();
	OnPlayReleaseSkillSound();

	GetWorldTimerManager().SetTimer(TimerHandle_Destroy,this,&ABoss_Skill::SkillDestroy,4.f,false);
}

// Called every frame
void ABoss_Skill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoss_Skill::SetHomingTarget()
{
	ARBaseCharacter* BaseCharacter = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	if(BaseCharacter)
	{
		ACharacter* Character = Cast<ACharacter>(BaseCharacter->GetParentActor());
		if(Character)
			ProjectileMovementComponent->HomingTargetComponent = Character->GetArrowComponent();
		ProjectileMovementComponent->HomingAccelerationMagnitude = 0;
	}
}

void ABoss_Skill::GetBoss()
{
	for(TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* FoundActor = *ActorItr;
		if(FoundActor->IsA(ABoss::StaticClass()))
		{
			Boss = Cast<ABoss>(FoundActor);
			break;
		}
	}
}

void ABoss_Skill::OnPlayReleaseSkillSound()
{
	if(LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,LaunchSound,GetActorLocation());
	}
}

void ABoss_Skill::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARBaseCharacter* BaseCharacter = Cast<ARBaseCharacter>(OtherActor);
	if(BaseCharacter && OtherComp == BaseCharacter->GetCapsuleComponent())
	{
		if(ImpactEffect)
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ImpactEffect,FVector(SweepResult.ImpactPoint),FRotator::ZeroRotator);
		if(HitSound)
			UGameplayStatics::PlaySoundAtLocation(this,HitSound,OtherActor->GetActorLocation());
		IDamageAbleInterface* DamageAbleInterface = Cast<IDamageAbleInterface>(BaseCharacter);
		if(DamageAbleInterface)
		{
			DamageAbleInterface->OnReceiveDamage(
				10.f,
				Boss->DamageType,
				Boss->BossElement,
				Boss->BossCritChance,
				Boss,
				nullptr);
			Destroy();
		}
	}
}

void ABoss_Skill::SkillDestroy()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Destroy);
	Destroy();
}

