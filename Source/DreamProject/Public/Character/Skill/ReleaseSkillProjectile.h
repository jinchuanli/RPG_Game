// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "ReleaseSkillProjectile.generated.h"

class UParticleSystem;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class DREAMPROJECT_API AReleaseSkillProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReleaseSkillProjectile();
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollsion;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// UPROPERTY(VisibleAnywhere)
	// class UParticleSystemComponent* ParticleSystem;
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* Niagaracomponent;
	
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;//控制移动的组件
	
	//持有的特效
	// UPROPERTY(EditAnywhere,Category=Effect)
	// UParticleSystem* MissileEffect; // 导弹的特效
	// UPROPERTY(EditAnywhere,Category=Effect)
	// UParticleSystem* ImpactEffect;//爆炸的特效

	UPROPERTY(EditAnywhere,Category=Effect)
	UNiagaraSystem* MissileEffect;
	UPROPERTY(EditAnywhere,Category=Effect)
	UNiagaraSystem* ImpactEffect;
	UPROPERTY(EditAnywhere,Category=Effect)
	USoundBase* Sound;
	

	class AReleaseSkill* Skill;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetVelocity();
	FTimerHandle TimerHandle_Destory;
	void DestroySkill();

	void SetRotation();
};
