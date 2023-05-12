// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/SkillEnum.h"
#include "GameFramework/Character.h"
#include "DamageAbleInterface/DamageAbleInterface.h"
#include "Interface/SelectAbleInterface.h"
#include "NormalEnemy.generated.h"

class ABaseElement;
UCLASS()
class DREAMPROJECT_API ANormalEnemy : public ACharacter,public IDamageAbleInterface,public ISelectAbleInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANormalEnemy();

	ANormalEnemy* EnemyCharacter;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	//
protected:
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category=BeHevior)
	bool bDead = false;

	//获取控制类
	class ANormalEnemyController* MyController;


	//94
	UPROPERTY(VisibleAnywhere,Category=AI)
	class UAIPerceptionComponent* AIPerceptionComp;
	class UAISenseConfig_Sight* SightConfig;

	//95
public:
	UPROPERTY(EditAnywhere,Category=BeHevior)  //在ue中赋值的攻击的动画
	TArray<class UAnimMontage*>AttackAnimations;

	//96
	FORCEINLINE bool GetbDead(){return bDead;}

	//100
	FVector StartLocation;  //初始的位置
protected:
	//101
	UFUNCTION()
	void OnSightPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	UPROPERTY(EditAnywhere,Category=Info)
	bool bAggressive; //是否是有侵略性的敌人

	//108
	protected:
	UPROPERTY(EditAnywhere,Category=Info)
	TSubclassOf<ABaseElement> Element;

	//111
	UFUNCTION(BlueprintCallable,Category=Spider)
	void NotifyHit_Spider();

	UPROPERTY(EditAnywhere,Category=Info)
	float BaseDamage = 25.f; //基础伤害
	UPROPERTY(EditAnywhere,Category=Info)
	EDamageType DamageType = EDamageType::Physical;//物理伤害

	int CritChance = 25;//暴击率

	//112
	//射线检测
	UFUNCTION(BlueprintCallable,Category=Spider)
	void AttackRay();
	UFUNCTION(BlueprintCallable,Category=Spider)
	void LAttackRay();

	//115
	UPROPERTY(VisibleAnywhere,Category=UI)  //暴露组件的细节
	class UWidgetComponent* EnemyWidgetComp;

	//116
	UPROPERTY(VisibleAnywhere,Category=UI)
	class USphereComponent* ShowUICollision;
	
	//117
	//当主角与球形碰撞体发生碰撞的时候，显示血条信息
	UFUNCTION()  //.AddDynamic动态绑定必须添加UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	bool bInShowRange;

	//118
	UPROPERTY(EditAnywhere,Category=Info)
	FText Name = FText::FromString("Spider");
	UPROPERTY(EditAnywhere,Category=Info)
	int Level = 1;

	class UEnemyInfoWidget* EnemyInfoWidget;

	void InitWidgetText();

	//117
public:
	UPROPERTY(EditAnywhere,Category=Info)
	float TotalHealth = 50.f;
	float CurrentHealth;
	void UpdateHealthBar();

protected:
	//120
	virtual void OnReceiveDamage(float FBaseDamage,EDamageType Type,TSubclassOf<ABaseElement> FElement,int FCritChance,AActor* Attacker,ABaseSkill* Spell) override;

	//121
	void OnDeath(AActor* Killer);
	UPROPERTY(EditAnywhere,Category=Info)
	float ExpForKill = 500;  // 杀死对像能够获得的经验

	//122
	UPROPERTY(EditAnywhere,Category=Info)
	bool bDoesRespawn = true;  //是否是能够重生的怪物类型
	void Respawn();  //重生
	FTimerHandle TimerHandle_Respawn;
	UPROPERTY(EditAnywhere,Category=Info)
	float RespawnTime = 10.f; //复活的时间

	//129
	virtual void OnSelected(class ARBaseCharacter* Character) override; // 选中的时候
	virtual void OnSelectionEnd(class ARBaseCharacter* Character) override; // 选择离开后调用
	bool bSelected = false;
	class ARBaseCharacter* SelectingCharacter;

	void OnDelayDeath();
	FTimerHandle TimerHandle_DelayDeath;

	//137
public:
	UPROPERTY(VisibleAnywhere,Category=Hit)
	class UArrowComponent* HitArrow;

	UPROPERTY(EditAnywhere,Category=Item)
	TArray<TSubclassOf<AActor>> ArrayOfActor;

	FTimerHandle TimerHandle_Destroy;
	void SpawnItem(TArray<TSubclassOf<AActor>> Item);
	void SpawnItemDestory();
	TArray<AActor*> SpawnActor;
	
	int32 IsHit = 0;
};
