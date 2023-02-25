// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/HotKeyRow.h"
#include "Components/HorizontalBox.h"
#include "H:\DreamProject\Source\DreamProject\Public\UserWidget\SkillHotKey.h"

TArray<USkillHotKey*> UHotKeyRow::GenerateHoyKeys()
{
	for(auto key:Keys)
	{
		USkillHotKey * SkillHotKey = CreateWidget<USkillHotKey>(GetWorld(),
		LoadClass<USkillHotKey>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/WBP_SkillHotKey.WBP_SkillHotKey_C'")));
		SkillHotKey->SetKey(key);
		SlotWidgets.Add(SkillHotKey);
		RowBox->AddChildToHorizontalBox(SkillHotKey);
	}
	return SlotWidgets;
}
