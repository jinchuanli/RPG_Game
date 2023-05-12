// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Skill/SkillTreeStruct.h"
#include "Components/ActorComponent.h"
#include "SkillTreeComponent.generated.h"

class ABaseSkill;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREAMPROJECT_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	class ARBaseCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere,Category=Skill)
	int SkillPoints = 2;//技能点

	UPROPERTY(EditAnywhere,Category=Skill)
	TArray<FTreeCategory> Categories;
	
	// Sets default values for this component's properties
	
	USkillTreeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bTreeSetup;//技能树中的内容是否已经设置好了

	class UMainTreeWidget* MainTreeWidget;
public:
	ABaseSkill* UpgradedSpell;
	class USkillTreeEntryWidget* EntryWidget;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool BPlayerLearnedSpell(TSubclassOf<ABaseSkill> Skill);  //主角是否学习了这个技能

	void AddSkillPoints(int Amount);

	bool BCanUpgradeSpell(ABaseSkill* Spell);

	bool BSkillOnHotkey(ABaseSkill* Spell);

	void UpgradeSpell(ABaseSkill* SkillActor,class USkillTreeEntryWidget* EntryWidget_F);
	void DowngradeSpell(ABaseSkill* SkillActor,class USkillTreeEntryWidget* EntryWidget);

	UFUNCTION()
	void HandleShowCommand();   //处理显示和隐藏的命令

	void SetUpTree();

	void ActiveConnections(TSubclassOf<ABaseSkill> ForSpell);
	void UpdateAllEntries();
};
