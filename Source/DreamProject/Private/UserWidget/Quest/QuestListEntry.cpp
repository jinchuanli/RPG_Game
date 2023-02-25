// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Quest/QuestListEntry.h"

#include "Character/RBaseCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Quest/BaseQuest.h"
#include "UserWidget/Quest/QuestJournal.h"

void UQuestListEntry::NativeConstruct()
{
	Super::NativeConstruct();
	Update();
	//动态绑定按键
	QuestButton->OnClicked.AddDynamic(this,&UQuestListEntry::OnQuestButtonClicked);
	UE_LOG(LogTemp,Warning,TEXT("NativeConstructQuestListEntry"));
}

void UQuestListEntry::UpdateLevelColor()
{
	FLinearColor TempColor;
	if(Journal->PlayerCharacter->GetCurrentLevel() > AssignedQuest->QuestInfo.SuggestedLevel)
	{
		TempColor = FLinearColor::Green;
	}
	else if(Journal->PlayerCharacter->GetCurrentLevel() == AssignedQuest->QuestInfo.SuggestedLevel)
	{
		TempColor = FLinearColor::White;
	}
	else
	{
		TempColor = FLinearColor::Red;
	}
	SuggestedLevel->SetColorAndOpacity(TempColor);
}

void UQuestListEntry::Update()
{
	if(AssignedQuest->QuestInfo.Name.ToString().Len() < 20)
	{
		SetQuestName(AssignedQuest->QuestInfo.Name);
	}
	else
	{
		SetQuestName(FText::FromString(AssignedQuest->QuestInfo.Name.ToString().LeftChop(AssignedQuest->QuestInfo.Name.ToString().Len() - 17).Append("...")));
	}

	SetSuggestedLevel(FText::AsNumber(AssignedQuest->QuestInfo.SuggestedLevel));
	UpdateLevelColor();
	//将地区枚举转换成字符串
	SetRegionName(FText::FromString(GetRegionEnumString(AssignedQuest->QuestInfo.Regions)));
}

void UQuestListEntry::SetQuestName(FText QuestNameText)
{
	QuestName->SetText(QuestNameText);
}

void UQuestListEntry::SetRegionName(FText RegionNameText)
{
	RegionName->SetText(RegionNameText);
}

void UQuestListEntry::SetSuggestedLevel(FText LevelText)
{
	SuggestedLevel->SetText(LevelText);
}

void UQuestListEntry::OnQuestButtonClicked()
{
	Journal->OnQuestClicked(this);
	UE_LOG(LogTemp,Warning,TEXT("点击任务列表"));
}

FString UQuestListEntry::GetRegionEnumString(ERegions Region)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE,TEXT("ERegions"),true);
	if(!EnumPtr)
	{
		return FString("InValid");
	}
	FString TempString = EnumPtr->GetNameByValue((int64)Region).ToString();
	TempString.RemoveFromStart("ERegions::");
	return TempString;
}
