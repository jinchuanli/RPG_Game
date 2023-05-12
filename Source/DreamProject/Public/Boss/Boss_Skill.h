// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss_Skill.generated.h"

class UNiagaraSystem;
class USoundBase;

UCLASS()
class DREAMPROJECT_API ABoss_Skill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoss_Skill();

	class ABoss* Boss;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* NiagaraComponent;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere,Category=Effect)
	UNiagaraSystem* MissileEffect;
	UPROPERTY(EditAnywhere,Category=Effect)
	UNiagaraSystem* ImpactEffect;
	UPROPERTY(EditAnywhere,Category=Effect)
	USoundBase* LaunchSound;
	UPROPERTY(EditAnywhere,Category=Effect)
	USoundBase* HitSound;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetHomingTarget();

	void GetBoss();

	UFUNCTION(BlueprintCallable)
	void OnPlayReleaseSkillSound();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	FTimerHandle TimerHandle_Destroy;
	void SkillDestroy();
};
