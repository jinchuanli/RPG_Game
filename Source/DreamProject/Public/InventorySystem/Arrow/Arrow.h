// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/SkillEnum.h"
#include "Character/Skill/BaseElement.h"
#include "InventorySystem/BaseItem.h"
#include "Arrow.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class DREAMPROJECT_API AArrow : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* ArrowMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USphereComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere,Category=WeaponInfo)
	int64 Damage;
	UPROPERTY(EditAnywhere,Category=WeaponInfo)
	EDamageType DamageType;
	UPROPERTY(EditAnywhere,Category=WeaponInfo)
	TSubclassOf<ABaseElement> BaseElement;
	UPROPERTY(EditAnywhere,Category=WeaponInfo)
	int32 CritChance;

	FTimerHandle TimerHandle_Destory;

	class ARBaseCharacter* BaseCharacter;
public:	
	AArrow();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Arrow")
	bool EnableDamege = false;
	
	UFUNCTION(BlueprintCallable,Category=Attack)
	void ArrowCreatDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DestoryArrow();
	
	FORCEINLINE UStaticMeshComponent* GetArrowMesh() { return ArrowMesh; };
	FORCEINLINE USphereComponent* GetSphere() { return CollisionComp; };
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() { return ProjectileMovement; };
};
