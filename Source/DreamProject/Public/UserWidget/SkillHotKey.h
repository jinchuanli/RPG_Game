// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputCoreTypes.h"
#include "SkillHotKey.generated.h"

class UTextBlock;
class UImage;
class UButton;
class ABaseSkill;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API USkillHotKey : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* HotKeyName;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* CoolDownText;

	UPROPERTY(Meta = (BindWidget))
	UImage* SkillIcon;
	UPROPERTY(Meta = (BindWidget))
	UImage* CoolDownImage;

	UPROPERTY(Meta = (BindWidget))
	UImage* Base;

	UPROPERTY(Meta = (BindWidget))
	UButton* SkillButton;
	
	UPROPERTY(BlueprintReadOnly,Category="My")
	FKey Key;//键盘按键，每一个SkillHotKey对应一个Key，用这个Key，即可释放SkillHotKey上的技能
	
	bool bHasSetKeyName = false;

	bool bDeactive;//是否失活
//
	bool bDraggedOver = false;  //标识当前是否正在被拖拽
	FLinearColor DragOverColor; //这个插槽为空的时候，有图标过来，显示的颜色
	FLinearColor DefaultColor; //默认颜色
//
	class UMaterialInstanceDynamic* CooldownMat;

	virtual bool Initialize() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	ABaseSkill* AssignedSpell;//分配的技能

	UFUNCTION()
	void OnSkillButtonClicked();

	//重设不放下去的时候技能图标的颜色
	void ResetStyle();

	//
	//返回鼠标右键按下的事件，只有当鼠标右键按下的时候才来执行检测拖拽的方法
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	//当检测到拖拽时调用的函数
	virtual void NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation )override;
	//当拖拽到另外一个UI上晃荡的时候，触发这个函数
	virtual bool NativeOnDragOver( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )override;
	//当拖拽的UI从一个UI上离开的时候触发
	virtual void NativeOnDragLeave( const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )override;
	//当放下的时候
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )override;
	UPROPERTY(EditAnywhere,Category=DragDrop)
	TSubclassOf<UDragDropOperation> SkillDragOperation;
	//
public:
	FORCEINLINE void SetKey(FKey FKey){this->Key = FKey;}//用来设置key的

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void AssignSpell(ABaseSkill* NewAssignSpell);//为该快捷键分配技能
	void ClearAssignedSpell();//清楚分配的技能

	void EnableHotKey();//激活快捷键，点击有效
	void DisableHotKey();//失活快捷键，技能在释放的时候不能在有效了
	FORCEINLINE ABaseSkill* GetAssignedSpell(){return AssignedSpell;}

	FORCEINLINE FKey GetKey(){return Key;}
};
