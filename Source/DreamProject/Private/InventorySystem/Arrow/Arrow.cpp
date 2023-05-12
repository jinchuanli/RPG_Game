// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Arrow/Arrow.h"

#include "Character/RBaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Enemy/NormalEnemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(1.0f);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable,0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	//CollisionComp->OnComponentHit.AddDynamic(this,&AArrow::OnHit);
	
	// 当碰撞发生时，调用 OnHit 函数
	SetRootComponent(CollisionComp);
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;

	// 默认不进行模拟，只有我们在接受到攻击后，才进行物理模拟
	ProjectileMovement->Velocity = (FVector(0,0,0));
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->bRotationFollowsVelocity = true;

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(CollisionComp);
	
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
	BaseCharacter = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));
}

void AArrow::ArrowCreatDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANormalEnemy* TempEnemy = Cast<ANormalEnemy>(OtherActor);
	if(TempEnemy && OtherComp == TempEnemy->GetCapsuleComponent())
	{
		//传递伤害
		IDamageAbleInterface* DamageAbleInterface = Cast<IDamageAbleInterface>(TempEnemy);
		if(DamageAbleInterface)
		{
			DamageAbleInterface->OnReceiveDamage(
				Damage,
				DamageType,
				BaseElement,
				CritChance,
				BaseCharacter,
				nullptr);
			EnableDamege = false;
		}
	}
	GetWorldTimerManager().SetTimer(TimerHandle_Destory,this,&AArrow::DestoryArrow,10.f,false);
}

void AArrow::DestoryArrow()
{
	this->Destroy();
	GetWorldTimerManager().ClearTimer(TimerHandle_Destory);
}


