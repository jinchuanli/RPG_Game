// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../../Plugins/Developer/RiderLink/Source/RD/thirdparty/clsocket/src/ActiveSocket.h"
#include "DamageAbleInterface/DamageAbleInterface.h"
#include "GameFramework/Character.h"
#include "Character/Skill/SkillEnum.h"
#include "Boss.generated.h"

class ABaseElement;
class UAnimMontage;
UCLASS()
class DREAMPROJECT_API ABoss : public ACharacter,public IDamageAbleInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	////////////////////////////////////////////////////////////////////////////////////////////

public:
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere,Category=BossInfo)
	bool bDead = false;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=BossInfo)
	TSubclassOf<ABaseElement> BossElement;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=BossInfo)
	float BossDamage = 25.f; //基础伤害
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=BossInfo)
	EDamageType DamageType = EDamageType::Physical;//物理伤害
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=BossInfo)
	int BossCritChance = 25;//暴击率
	UPROPERTY(EditAnywhere,Category=BossInfo)
	FText Name = FText::FromString("Boss");
	UPROPERTY(EditAnywhere,Category=BossInfo)
	int Level = 1;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=BossInfo)
	float TotalHealth = 50.f;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=BossInfo)
	float CurrentHealth = TotalHealth;
	UPROPERTY(EditAnywhere,Category=BossInfo)
	bool bDoesRespawn = true;  //是否是能够重生的怪物类型
	UPROPERTY(EditAnywhere,Category=BossInfo)
	float Receive;
	UPROPERTY(EditAnywhere,Category=BossInfo)
	UAnimMontage* DeadAnim;

	class AController* AIController;


	UPROPERTY(EditAnywhere,Category=SpawnItem)
	TArray<TSubclassOf<AActor>> SpawnItemArray;
	
	FTimerHandle TimerHandle_ItemDestroy;
	
	TArray<AActor*> SpawnActorArray;
	

public:
	virtual void OnReceiveDamage(float BaseDamage,EDamageType Type,TSubclassOf<ABaseElement> Element,int CritChance,AActor* Attacker,ABaseSkill* Spell) override;
	
	UFUNCTION(BlueprintCallable,Category=Attack)
	void CreatDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void BossDead();

	FTimerHandle TimerHandle_Destroy;
	void DestroyBoss();

	void SpawnItem(TArray<TSubclassOf<AActor>> Item);
	void SpawnItemDestroy();
};
