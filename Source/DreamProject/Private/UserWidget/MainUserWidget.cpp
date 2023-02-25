// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/MainUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/VerticalBox.h"
#include "UserWidget/HotKeyRow.h"
#include "Components/Button.h"
#include "UserWidget/SkillDragOperation.h"
#include "UserWidget/Quest/QuestJournal.h"
#include "UserWidget/Quest/QuestWidget.h"
#include "UserWidget/SkillTree/MainTreeWidget.h"

//初始化注册
bool UMainUserWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	LevelText = Cast<UTextBlock>(GetWidgetFromName("Text_level"));
	HPPProgressBar = Cast<UProgressBar>(GetWidgetFromName("ProgressBar_HP"));
	MPPProgressBar = Cast<UProgressBar>(GetWidgetFromName("ProgressBar_MP"));

	Button_Quest->OnClicked.AddDynamic(this,&UMainUserWidget::OnQuestButtonClicked);
	Button_Skill->OnClicked.AddDynamic(this,&UMainUserWidget::OnSkillButtonClicked);
	return true;
}

void UMainUserWidget::SetLevelText(FText Text)
{
	LevelText->SetText(Text);
}

void UMainUserWidget::SetHPPProgressBar(float Percent)
{
	HPPProgressBar->SetPercent(Percent);
}

void UMainUserWidget::SetMPPProgressBar(float Percent)
{
	MPPProgressBar->SetPercent(Percent);
}

void UMainUserWidget::GenerateHotKeys(TArray<FKey> KeysToGenerate, int32 KeysPerRow)
{
	TArray<FKey> LocalKeys = KeysToGenerate;
	TArray<FKey> LocalModifyKeys = KeysToGenerate;
	if(KeysToGenerate.Num() % KeysPerRow != 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("The num of keys is invalid"));
		return;
	}
	//槽位刚刚好
	if(LocalKeys.Num() == KeysPerRow)
	{
		UHotKeyRow* HotKeyRow = CreateWidget<UHotKeyRow>(GetWorld(),
			LoadClass<UHotKeyRow>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/WBP_HpyKeyRow.WBP_HpyKeyRow_C'")));
		HotKeyRow->SetHotkeys(LocalKeys);
		//HotkeyRowContainer->AddChildToVerticalBox(HotKeyRow);
		AllHotkeySlots.Append(HotKeyRow->GenerateHoyKeys());
		HotKeyRows.Add(HotKeyRow);
		for(int i = HotKeyRows.Num() - 1; i >= 0; i--)
		{
			HotkeyRowContainer->AddChildToVerticalBox(HotKeyRows[i]);
		}
	}
	else//多余另起一行
	{
		LocalModifyKeys.SetNum(KeysPerRow);
		UHotKeyRow* HotKeyRow = CreateWidget<UHotKeyRow>(GetWorld(),
			LoadClass<UHotKeyRow>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/WBP_HpyKeyRow.WBP_HpyKeyRow_C'")));
		HotKeyRow->SetHotkeys(LocalModifyKeys);
		//HotkeyRowContainer->AddChildToVerticalBox(HotKeyRow);
		AllHotkeySlots.Append(HotKeyRow->GenerateHoyKeys());
		HotKeyRows.Add(HotKeyRow);
		for(auto Key : LocalModifyKeys)
		{
			if(LocalKeys.Contains(Key))
			{
				LocalKeys.Remove(Key);
			}
		}
		GenerateHotKeys(LocalKeys,KeysPerRow);
	}
}

void UMainUserWidget::PlayPopUpAnimation_Implementation()
{
}

bool UMainUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if(Cast<USkillDragOperation>(InOperation))
	{
		if(Cast<USkillDragOperation>(InOperation)->FromHotKey)
			if(Cast<USkillDragOperation>(InOperation)->FromHotKey)
				Cast<USkillDragOperation>(InOperation)->FromHotKey->ClearAssignedSpell();
		UE_LOG(LogTemp,Warning,TEXT("调用丢到主界面"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Cast<USkillDragOperation>(InOperation)不存在！"));
		return false;
	}
}

void UMainUserWidget::SetExpPanel(float CurrentExp, float NeedExpToNextLevel)
{
	ExpBar->SetPercent(CurrentExp / NeedExpToNextLevel);
	CurrentExpText->SetText(FText::AsNumber(CurrentExp));
	NeedExpToNextLevelText->SetText(FText::AsNumber(NeedExpToNextLevel));
}

UQuestWidget* UMainUserWidget::AddQuestToList(ABaseQuest* Quest)
{
	if(Quest)
	{
		UQuestWidget* QuestWidget = CreateWidget<UQuestWidget>(GetWorld(),
			LoadClass<UQuestWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Quest/WBP_Quest.WBP_Quest_C'")));
		QuestWidget->QuestManager = QuestManager;
		QuestWidget->AssignedQuest = Quest;
		QuestWidgets.Add(QuestWidget);

		UScrollBoxSlot* Temp = Cast<UScrollBoxSlot>(QuestList->AddChild(QuestWidget));
		Temp->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		Temp->SetPadding(FMargin(0,8,0,0));
		UE_LOG(LogTemp,Warning,TEXT("添加任务到列表成功"));
		return QuestWidget;
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("添加任务到列表失败"));
		return nullptr;
	}
}

void UMainUserWidget::OnQuestButtonClicked()
{
	if(bQuestVisibling)
	{
		QuestJournal->SetVisibility(ESlateVisibility::Hidden);
		bQuestVisibling = false;
	}
	else
	{
		QuestJournal->SetVisibility(ESlateVisibility::Visible);
		bQuestVisibling = true;
	}
}

void UMainUserWidget::OnSkillButtonClicked()
{
	if(bTreeShown)
	{
		SkillTree->SetVisibility(ESlateVisibility::Hidden);
		bTreeShown = false;
	}
	else
	{
		SkillTree->SetVisibility(ESlateVisibility::Visible);
		bTreeShown = true;
	}
}

