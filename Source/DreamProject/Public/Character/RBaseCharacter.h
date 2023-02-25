// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "DamageAbleInterface/DamageAbleInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Quest/QuestEnum.h"
#include "Skill/SkillEnum.h"
#include "RBaseCharacter.generated.h"

class ABaseSkill;
class UTexture2D;
class ABaseElement;
class UBuffWidget;
class ABuffSkill;

//技能的阶段(等级),阶段不同属性不同
USTRUCT()
struct FSkillStage
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere,Category=SkillData)
	int32 RequireLevel;//需要前置的等级
	
	UPROPERTY(EditAnywhere,Category=SkillData)
	TArray<TSubclassOf<ABaseSkill>> RequireSkills; //需要的前置技能

	UPROPERTY(EditAnywhere,Category=SkillData)
	UTexture2D* OverrideIcon; //覆盖的图标

	UPROPERTY(EditAnywhere,Category=SkillData)
	float Range;//技能的伤害范围

	UPROPERTY(EditAnywhere,Category=SkillData)
	float Damage;//技能的伤害值

	UPROPERTY(EditAnywhere,Category=SkillData)
	EDamageType DamageType;//伤害类型

	UPROPERTY(EditAnywhere,Category=SkillData)
	int32 CriticalChance;// 暴击率

	UPROPERTY(EditAnywhere,Category=SkillData)
	float CooldownTime;//冷却时间

	UPROPERTY(EditAnywhere,Category=SkillData)
	float CastingTime;//释放的时间

	UPROPERTY(EditAnywhere,Category=SkillData)
	float ManaCost;//魔法消耗时间

	UPROPERTY(EditAnywhere,Category=SkillData)
	float MisiileSpeed;//投掷类技能的速度
};

//技能的信息
USTRUCT()
struct FSkillInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = SkillData)
	FText Name;
	UPROPERTY(EditAnywhere, Category = SkillData)
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere, Category = SkillData)
	FText Description; //技能的描述
	UPROPERTY(EditAnywhere, Category = SkillData)
	TSubclassOf<ABaseElement> Element; //该技能属于哪一个元素
	UPROPERTY(EditAnywhere, Category = SkillData)
	ETargetTypes Target; //作用对象
	UPROPERTY(EditAnywhere, Category = SkillData)
	TArray<FSkillStage> Stages; //技能所拥有的所有阶段，通过SkillInfo来获取SkillStage
};

UCLASS()
class DREAMPROJECT_API ARBaseCharacter : public ACharacter,public IDamageAbleInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARBaseCharacter();

public:
	UPROPERTY(BlueprintReadOnly)
	class UMainUserWidget* MainUserWidget;
	class APlayerController *PC;
protected:
	//摄像机组件
	UPROPERTY(VisibleAnywhere,Category="Camera")
	class UCameraComponent *FollowCamera;

	//弹簧臂组件  VisibleAnywhere不仅能看见还可以改变值
	UPROPERTY(VisibleAnywhere,Category="Camera")
	class USpringArmComponent *CameraBoom;

	//TSubclassOf 赋值的时候赋值到子类
	UPROPERTY(EditAnywhere,Category=Mouse)
	TSubclassOf<class ACursorDecal> CursorDecal;

	bool bHasMouseMoveCommand;

	class ACursorDecal *CurrentCursor;
	
	UPROPERTY(EditAnywhere,Category=Camera)
	float MinCameraZoom_V = 75.f;
	UPROPERTY(EditAnywhere,Category=Camera)
	float MaxCameraZoom_V = 1000.f;
	UPROPERTY(EditAnywhere,Category=Camera)
	float CameraZoomAlpha = 25.f;

	UPROPERTY(VisibleAnywhere,Category=Camera)
	class USceneCaptureComponent2D* PortraitComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void OnSetDestinationPressed();

	void SetNewMoveDestination(const FVector DesLocation);
	
	void MoveToCursor();
	//取消鼠标移动
	void CancleMoveToCursor();

	void CameraZoomIn();
	void CameraZoomOut();

protected:
	UPROPERTY(VisibleAnywhere,Category=PlayerInfo)
	FString CharacterName;
	
	UPROPERTY(VisibleAnywhere,Category=PlayerInfo)
	float TotalHp;
	UPROPERTY(VisibleAnywhere,Category=PlayerInfo)
	float TotalMp;
	UPROPERTY(VisibleAnywhere,Category=PlayerInfo)
	float CurrentHp;
	UPROPERTY(VisibleAnywhere,Category=PlayerInfo)
	float CurrentMp;
	UPROPERTY(VisibleAnywhere,Category=PlayerInfo)
	float CurrentExp;
	UPROPERTY(VisibleAnywhere,Category=PlayerInfo)
	int32 CurrentLevel;

	//读取数据
	void ReadData();
	
protected:
	UPROPERTY(EditAnywhere,Category=UI)
	TArray<FKey> Keys;
	UPROPERTY(EditAnywhere, Category = UI)
	int32 KeysPerRow;

	// UPROPERTY(EditAnywhere,Category=Skill)
	// TArray<TSubclassOf<ABaseSkill>> StartingSkills;//初始技能

	UPROPERTY(BlueprintReadOnly)
	ABaseSkill* CurrentSkill;//当前技能
	//void GenerateStartingSkills();//生成初始技能
	//
	bool bCanFindKey = true;
	void OnAnyKeyPressed(FKey Key);//按下任意键释放技能 key是当前按下的键
	//

	//保证一次性之能释放一个技能，有技能释放就不能同时释放别的技能
	bool bIsCasting;  //是否在释放技能
	
public:
	void BeginSpellCast(ABaseSkill* Skill);
	void EndSpellCast(ABaseSkill* Skill);
	//
	
public:	


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//内联函数
	FORCEINLINE float GetCurrentHp() {return CurrentHp;}
	void ChangeCurrentHp(float DeltaHp);

	void ChangeCurrentMp(float DeltaMp);
	FORCEINLINE float GetCurrentMp() {return CurrentMp;}

	void IncreaseCurrentExp(float DeltaExp);

	FORCEINLINE void SetCharacterName(FString Name){CharacterName = Name;}
	FORCEINLINE FString GetCharacterName(){return CharacterName;}

	FORCEINLINE bool GetbIsCasting(){return bIsCasting;}



public:
	//106
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//108
protected:
	UPROPERTY(EditAnywhere,Category=Skill)
	TSubclassOf<ABaseElement> Element;

public:

	//接收伤害
	virtual void OnReceiveDamage(float BaseDamage,EDamageType Type,TSubclassOf<ABaseElement> Elements,int CritChance,AActor* Attacker,ABaseSkill* Spell) override;

	//130
protected:
	AActor* SelectActor;//之前被点击的物体

public:
	//113
	class ANormalEnemy* SelectedEnemy;

protected:
	//139
	void CancleMissile(); // 取消导弹类技能的寻路
public:
	class AMissileSkill* MissileSkill;

public:  //BUff
	TArray<ABuffSkill*> Buffs;
	UBuffWidget* BuffWidget;

	UBuffWidget* AddBuff(ABuffSkill* SkillBuff);
	void RemoveBuff(ABuffSkill* SkillBuff);




	////////////////////////////////////////////////////////////////////////////////////////////////Exp
protected:
	void IncreasedLevel();
	UPROPERTY(VisibleAnywhere,Category=Data)
	float NeededExpToNextLevel;




	////////////////////////////////////////////////////////////////////////////////////////SkillTree
protected:
	UPROPERTY(VisibleAnywhere)
	class USkillTreeComponent* SkillTreeComp;
	
public:
	TArray<TSubclassOf<ABaseSkill>> LearntSkills;//主角已经学习过了的技能
	FORCEINLINE int GetCurrentLevel(){return CurrentLevel;};

	//172
	void ToogleShowSkillTree();  //显示技能树

	void ToogleShowQuest();   //显示quest


	///////////////////////////////////////////////////////////////////////////////////////////////////////quest
	UPROPERTY(EditAnywhere,Category=Quest)
	TSubclassOf<class AQuestManager> QuestManagerClass;

	UPROPERTY(BlueprintReadOnly)
	class AQuestManager* QuestManager;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* InteractionComp;

protected:
	UFUNCTION()
	void OnInterActionCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnInterActionCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	void InteractToNPC();

	//早南图中实现
	UFUNCTION(BlueprintImplementableEvent)   //BlueprintImplementableEvent在南图中没有实现的话，将自动跳过，不会在C++中寻找函数
	void PlaySlideOutAnim();

	void TestCompleteQuest();
	void TestFailQuest();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAddNewQuestAnim();

	//测试打印枚举
	// FString GetRegionEnumString(ERegions Region);

public:////捡起任务道具
	TArray<TSubclassOf<class ABaseQuestProp>> ObtainedProps;


protected:
	//升级的特效
	UPROPERTY(EditAnywhere,Category=Particle)
	class UParticleSystem* LevelUpParticle;



	//////////////////////////////////////////////////////////////////////////////inventory
protected:
	UPROPERTY(EditAnywhere,Category=Inventory)
	TSubclassOf<class AInventory> InventoryClass;

public:
	class AInventory* InventoryRef;
};
