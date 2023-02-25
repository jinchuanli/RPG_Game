// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputCoreTypes.h"
#include "HotKeyRow.generated.h"

class USkillHotKey;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UHotKeyRow : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Meta = (BindWidget))
	class UHorizontalBox* RowBox; //存放这一排被创建的skillhotkey

	TArray<FKey> Keys; //这一排所要的按键，是从外部传递过来的，创建该HotkeyRow之后被赋值

	TArray<USkillHotKey*> SlotWidgets;  //这一排拥有的SkillHotKey；

public:
	FORCEINLINE void SetHotkeys(TArray<FKey> AKeys){this->Keys = AKeys;}

	TArray<USkillHotKey*> GenerateHoyKeys();
};
