// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillTreeComponent.h"
#include "Character/RBaseCharacter.h"
#include "Character/Skill/BaseSkill.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/SkillHotKey.h"
#include "UserWidget/SkillTree/ConnectionWidget.h"
#include "UserWidget/SkillTree/MainTreeWidget.h"
#include "UserWidget/SkillTree/SkillTreeEntryWidget.h"
#include "UserWidget/SkillTree/SubTreeWidget.h"
#include "Character/BasePlayerController.h"

// Sets default values for this component's properties
USkillTreeComponent::USkillTreeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ARBaseCharacter>(GetOwner());
	
}


// Called every frame
void USkillTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USkillTreeComponent::BPlayerLearnedSpell(TSubclassOf<ABaseSkill> Skill)
{
	return PlayerCharacter->LearntSkills.Contains(Skill);
}

void USkillTreeComponent::AddSkillPoints(int Amount)
{
	SkillPoints += Amount;
}

bool USkillTreeComponent::BCanUpgradeSpell(ABaseSkill* Spell)
{
	//1,技能点需要大于0，2，主角当前的等级大于该技能下一阶段所需要的等级，3，限制技能点的最大值
	if(SkillPoints > 0 && PlayerCharacter->GetCurrentLevel() >= Spell->GetNextStage().RequireLevel &&
		Spell->GetCurrentStageIndex() < Spell->GetSkillInfo().Stages.Num() - 1)
	{
		bool LocalBool = true;
		for(TSubclassOf<ABaseSkill> PreSkill : Spell->GetNextStage().RequireSkills)
		{
			if(!BPlayerLearnedSpell(PreSkill))
			{
				//有的技能需要很多前置技能，如果有一个没有学习，则返回假
				LocalBool = false;
				break;
			}
		}
		return LocalBool;
	}
	else
	{
		return false;
	}
}

bool USkillTreeComponent::BSkillOnHotkey(ABaseSkill* Spell)
{
	bool LocalBool = false;
	for(USkillHotKey* TempHotkey : PlayerCharacter->MainUserWidget->GetAllHotkeySlots())
	{
		if(TempHotkey->GetAssignedSpell() && TempHotkey->GetAssignedSpell() == Spell)
		{
			LocalBool = true;
			break;
		}
	}
	return LocalBool;
}

void USkillTreeComponent::UpgradeSpell(ABaseSkill* SkillActor, USkillTreeEntryWidget* EntryWidget_F)
{
	this->UpgradedSpell = SkillActor;
	this->EntryWidget = EntryWidget_F;

	if(BCanUpgradeSpell(SkillActor))
	{
		SkillPoints -= 1;
		UpgradedSpell->ChangeCurrentStageIndex(1);
		if(UpgradedSpell->GetCurrentStageIndex() == 0)
		{
			PlayerCharacter->LearntSkills.Add(UpgradedSpell->GetClass());
			EntryWidget->OnSpellLearnt();
			ActiveConnections(SkillActor->GetClass());

			EntryWidget->UpdateStageText();
			EntryWidget->UpdateIcon();
			MainTreeWidget->UpdateSp();
			UpdateAllEntries();
		}
		else
		{
			EntryWidget->UpdateStageText();
			EntryWidget->UpdateIcon();
			MainTreeWidget->UpdateSp();
			UpdateAllEntries();
		}
	}
}

void USkillTreeComponent::DowngradeSpell(ABaseSkill* SkillActor, USkillTreeEntryWidget* EntryWidget_F)
{
	this->UpgradedSpell = SkillActor;
	this->EntryWidget = EntryWidget_F;

	UpgradedSpell->ChangeCurrentStageIndex(-1);
	SkillPoints += 1;
	EntryWidget->UpdateStageText();
	EntryWidget->UpdateIcon();
	MainTreeWidget->UpdateSp();
	UpdateAllEntries();
}

void USkillTreeComponent::HandleShowCommand()
{
	if(bTreeSetup)
	{
		if(PlayerCharacter->MainUserWidget->bTreeShown)
		{
			MainTreeWidget->SetVisibility(ESlateVisibility::Hidden);
			PlayerCharacter->MainUserWidget->bTreeShown = false;
			FInputModeGameOnly InputMode;
			PlayerCharacter->PC->bEnableClickEvents = false;
			PlayerCharacter->PC->bShowMouseCursor = false;
			PlayerCharacter->PC->SetInputMode(InputMode);
		}
		else
		{
			MainTreeWidget->SetVisibility(ESlateVisibility::Visible);
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerCharacter->PC->SetInputMode(InputMode);
			PlayerCharacter->PC->bEnableClickEvents = true;
			PlayerCharacter->PC->bShowMouseCursor = true;
			PlayerCharacter->MainUserWidget->bTreeShown = true;
		}
	}
}

void USkillTreeComponent::SetUpTree()
{
	MainTreeWidget = PlayerCharacter->MainUserWidget->SkillTree;
	if(MainTreeWidget)
	{
		MainTreeWidget->Initialze(this);
		bTreeSetup = true;
	}
}

void USkillTreeComponent::ActiveConnections(TSubclassOf<ABaseSkill> ForSpell)
{
	for(USubTreeWidget* SubTreeWidget : MainTreeWidget->SubTreeWidgets)
	{
		for(UConnectionWidget* ConnectionWidget : SubTreeWidget->Connections)
		{
			if(ConnectionWidget->ForSpell  == ForSpell)
			{
				ConnectionWidget->Activate();
			}
		}
	}
}

void USkillTreeComponent::UpdateAllEntries()
{
	for(USubTreeWidget* SubTreeWidget : MainTreeWidget->SubTreeWidgets)
	{
		for(USkillTreeEntryWidget* SkillTreeEntry : SubTreeWidget->SkillEntries)
		{
			SkillTreeEntry->UpdateUpgradeBox();
		}
	}
}

