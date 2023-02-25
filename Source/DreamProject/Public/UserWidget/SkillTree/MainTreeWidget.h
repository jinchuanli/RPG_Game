// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Skill/SkillTreeStruct.h"
#include "MainTreeWidget.generated.h"

class UTextBlock;
class UScrollBox;
class UButton;
class UWidgetSwitcher;
/**
 * 技能树主容器
 */
UCLASS()
class DREAMPROJECT_API UMainTreeWidget : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta=(BindWidget))
	UTextBlock* SpText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* LevelText;
	UPROPERTY(meta=(BindWidget))
	UScrollBox* CategoriesBox;
	UPROPERTY(meta=(BindWidget))
	UButton* CloseButton;
	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* SubTreeSwitcher;

	virtual void NativeConstruct() override;

public:
	class USkillTreeComponent* SkillTreeComp;
	int CurrentlySelectedIndex = -1; //当前选择的Category所对应的索引
	TArray<class UCategoryWidget*> CategoryWidgets;  //分类，比如skill，Defense
	TArray<FTreeCategory> TreeCategories;  //结构体， CategoryWidget的信息

	TArray<class USubTreeWidget*> SubTreeWidgets;//Category所对应的Content

//切换Switch的子组件，更具索引值
	void OnCategoryClicked(int Index);

	UFUNCTION()
	void OnCloseButtonClicked();


	void UpdateLevel();
	void UpdateSp();  //更新技能点

	void GenerateCategories();   //生成技能上面的分类

	void Initialze(class USkillTreeComponent* AssignedTreeComp);
};
