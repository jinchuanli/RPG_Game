// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicProjectile.generated.h"

class UParticleSystem;
UCLASS()
class DREAMPROJECT_API AMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//声明组件，检测碰撞
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollsion;
	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ParticleSystem;
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;//控制移动的组件

	//持有的特效
	UPROPERTY(EditAnywhere,Category=Effect)
	UParticleSystem* MissileEffect; // 导弹的特效
	UPROPERTY(EditAnywhere,Category=Effect)
	UParticleSystem* ImpactEffect;//爆炸的特效
	
	//
	class AMissileSkill* Skill;

	//140
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
