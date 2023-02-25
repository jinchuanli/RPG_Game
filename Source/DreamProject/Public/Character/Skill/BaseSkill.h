// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RBaseCharacter.h"
#include "GameFramework/Actor.h"
#include "BaseSkill.generated.h"

class ARBaseCharacter;
UCLASS()
class DREAMPROJECT_API ABaseSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category=SkillInfo)
	FSkillInfo SkillInfo;

	int32 CurrentStageIndex = -1;
	
	bool bOnCoolDown;//是否在冷却中
	bool bCurrentCasting;//技能是否正在释放

	class USkillHotKey* HotKey;//与该技能绑定的快捷键
 
	ARBaseCharacter* PlayerReference; //谁释放了该技能

	virtual void InitializeSpellCast();//初始技能释放,来判断魔法值是否足够
	virtual void OnSpellCast();//技能正在释放的时候，播放释放技能的动画
	virtual void OnCastCompleted();//技能结束

	//计时器，技能冷却
	FTimerHandle TimerHandle_CoolDown;
	virtual void OnCoolDown();

	//CD
	float CurrentCD;
	void OnCoolDownExpired();//技能冷却结束

	UAnimInstance* AnimInstance;
	//定义动画蒙太奇
	UPROPERTY(EditAnywhere,Category=SkillAnimation)
	UAnimMontage* SkillAnimMontage;

	FTimerHandle TimerHandle_ResetMove;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE FSkillStage GetCurrentStage(){return SkillInfo.Stages[FMath::Clamp(CurrentStageIndex,0,SkillInfo.Stages.Num()-1)];}
	FORCEINLINE FSkillStage GetNextStage(){return SkillInfo.Stages[FMath::Clamp(CurrentStageIndex+1,0,SkillInfo.Stages.Num()-1)];}
	
	FORCEINLINE void SethotKey(class USkillHotKey* FHotKey){this->HotKey = FHotKey;}

	FORCEINLINE FSkillInfo GetSkillInfo(){return SkillInfo;}

	FORCEINLINE bool GetBOnCoolDown(){return bOnCoolDown;}

	FORCEINLINE ARBaseCharacter* GetPlayerReference(){return PlayerReference;}
	FORCEINLINE void SetPlayerReference(ARBaseCharacter* FPlayerReference){this->PlayerReference = FPlayerReference;}
	//尝试释放技能
	UFUNCTION(BlueprintCallable)  //函数在南图中进行调用
	virtual void OnTryCastSpell();

	virtual void PlaySkillAnimation(UAnimMontage* SkillAnimtion);
	void RestMovement();
	
	UFUNCTION(BlueprintCallable)
	virtual void OnSkillNotify();

	//////////////////////////////////////////////////////////////////////////////////////////////////skilltree
	///
	FORCEINLINE int GetCurrentStageIndex(){return CurrentStageIndex;}

	void ChangeCurrentStageIndex(int DeltaIndex){CurrentStageIndex = FMath::Clamp(CurrentStageIndex + DeltaIndex,0,SkillInfo.Stages.Num()-1);}
};
