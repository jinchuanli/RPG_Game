// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/BaseElement.h"
#include "Character/Skill/SkillEnum.h"
#include "InventorySystem/BaseItem.h"
#include "ItemStaff.generated.h"

/**
 * 
 */

UENUM()
enum class EWeaponType
{
	Sword,
	Hammer,
	BowAndArrow
};

UCLASS()
class DREAMPROJECT_API AItemStaff : public ABaseItem
{
	GENERATED_BODY()


	virtual void OnUsed() override;
	
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="WeaponType")
	EWeaponType WeaponType;
	
	UFUNCTION(BlueprintCallable,Category=Attack)
	void CreatDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere,Category=WeaponInfo)
	int64 Damage;
	UPROPERTY(EditAnywhere,Category=WeaponInfo)
	EDamageType DamageType;
	UPROPERTY(EditAnywhere,Category=WeaponInfo)
	TSubclassOf<ABaseElement> BaseElement;
	UPROPERTY(EditAnywhere,Category=WeaponInfo)
	int32 CritChance;

	UFUNCTION(BlueprintCallable,Category="Weapon")
	bool EuqlEnum(FString Text);
	
};

