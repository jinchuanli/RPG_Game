// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SkillTreeEntryWidget.generated.h"

class UOverlay;
class UImage;
class UTextBlock;
class UVerticalBox;
class UButton;
/**
 * 技能插槽图标
 */
UCLASS()
class DREAMPROJECT_API USkillTreeEntryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UOverlay* SkillOverlay;
	UPROPERTY(meta=(BindWidget))
	UImage* SkillIcon;
	UPROPERTY(meta=(BindWidget))
	UImage* SkillBorder;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* StageText;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* UpgradeBox;
	UPROPERTY(meta=(BindWidget))
	UButton* PlusButton;
	UPROPERTY(meta=(BindWidget))
	UButton* MinusButton;
	
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USkillDragOperation> SkillDragOperation;

public:
	class USubTreeWidget* SubTree;
	class ABaseSkill* SkillActor;
	TSubclassOf<class ABaseSkill> SkillClass;
	bool bSpellLearnt;
	
	int GetAmountOfStages();//返回当前的技能有几个阶段
	void UpdateStageText();
	void UpdateIcon();  //更新升级后图标的函数
	void UpdateUpgradeBox();
	void OnSpellLearnt();


	UFUNCTION()
	void OnPlusButtonClicked();
	UFUNCTION()
	void OnMinusButtonClicked();

	//返回鼠标右键按下的事件，只有当鼠标右键按下的时候才来执行检测拖拽的方法
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	//当检测到拖拽时调用的函数
	virtual void NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation )override;
};
