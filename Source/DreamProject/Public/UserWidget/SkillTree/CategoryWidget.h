// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CategoryWidget.generated.h"

/**
 * 技能树的种类，包括技能，防御，buff
 */
UCLASS()
class DREAMPROJECT_API UCategoryWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	class UButton* CategoryButton;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CategoryNameText;
	
	//绑定按钮的函数
	UFUNCTION()
	void OnCategoryButtonHoverd();  //鼠标放上按键的时候//////////并不是点击
	UFUNCTION()
	void OnCategoryButtUnHoverd();   //鼠标移开按键的时候


	UFUNCTION()  //鼠标点击按键的时候
	void OnCategoryButtonClicked();

public:
	void ChangeState(bool FbSelected);

	FText CategoryName;
	bool bSelected;  //当前的Category是否被选择或者点击
	int Index;  //当前Category所对应的索引,不同种类的技能页面

	class UMainTreeWidget* MainTreeRef;
};
