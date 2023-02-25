// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate/WidgetTransform.h"
#include "UObject/NoExportTypes.h"
#include "SkillTreeStruct.generated.h"

USTRUCT()
struct FSubTreeConnection
{
	//技能间的连线
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere,Category=Skill)
	FVector2D Position;
	UPROPERTY(EditAnywhere,Category=Skill)
	FVector2D size;
	UPROPERTY(EditAnywhere,Category=Skill)
	FWidgetTransform Transform;
	UPROPERTY(EditAnywhere,Category=Skill)
	TSubclassOf<class ABaseSkill> ForSpell; //该连线所链接的前置技能
};

USTRUCT()
struct FSubTreeEntry
{
	//对应SkillTreeEntryWidget
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,Category=Skill)
	FVector2D Position;
	UPROPERTY(EditAnywhere,Category=Skill)
	TSubclassOf<class ABaseSkill> SpellClass; //该Entry所对应的技能
};


USTRUCT()
struct FSubTreeContent
{
	//对应USubTreeWidget，保存所有技能和连线
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere,Category=Skill)
	TArray<FSubTreeEntry> Skills;
	UPROPERTY(EditAnywhere,Category=Skill)
	TArray<FSubTreeConnection> Connections;
};

USTRUCT()
struct FTreeCategory
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,Category=Skill)
	FText Name;
	UPROPERTY(EditAnywhere,Category=Skill)
	FSubTreeContent Content;  //该按钮所对应的SubTreeContent
};
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API USkillTreeStruct : public UObject
{
	GENERATED_BODY()
	
};
