// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/MainUserWidget.h"

#include "Character/RBaseCharacter.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/VerticalBox.h"
#include "UserWidget/HotKeyRow.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/WrapBox.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/SettingWidget.h"
#include "UserWidget/SkillDragOperation.h"
#include "UserWidget/Inventory/InventoryDragDropOperation.h"
#include "UserWidget/Inventory/InventorySlotWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UserWidget/Inventory/ItemDragDropOperation.h"
#include "UserWidget/Inventory/ItemHotKey.h"
#include "UserWidget/Inventory/ItemObtainWidget.h"
#include "UserWidget/Inventory/ThrowWidget.h"
#include "UserWidget/Quest/QuestJournal.h"
#include "UserWidget/Quest/QuestWidget.h"
#include "Components/SkillTreeComponent.h"
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
	Button_Skill->OnClicked.AddDynamic(this,&UMainUserWidget::SkillTreeShow);
	Button_Inventory->OnClicked.AddDynamic(this,&UMainUserWidget::OnInventoryButtonClicked);
	Button_Setting->OnClicked.AddDynamic(this,&UMainUserWidget::OnSettingButtonClicked);
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
	
	if(Cast<USkillDragOperation>(InOperation))  //技能丢弃到主界面上
	{
		if(Cast<USkillDragOperation>(InOperation)->FromHotKey)
			if(Cast<USkillDragOperation>(InOperation)->FromHotKey)
				Cast<USkillDragOperation>(InOperation)->FromHotKey->ClearAssignedSpell();
		UE_LOG(LogTemp,Warning,TEXT("调用丢到主界面"));
		return true;
	}
	else if (Cast<UInventoryDragDropOperation>(InOperation))  //背包的拖拽
	{
		UE_LOG(LogTemp,Warning,TEXT("拖拽任务窗口执行"));
		Cast<UInventoryDragDropOperation>(InOperation)->WidgetToDrag->AddToViewport();
		Cast<UInventoryDragDropOperation>(InOperation)->WidgetToDrag->SetDesiredSizeInViewport(FVector2D(800,600));

		Cast<UInventoryDragDropOperation>(InOperation)->WidgetToDrag->SetPositionInViewport(InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition())
			- Cast<UInventoryDragDropOperation>(InOperation)->MouseOffest,false);
		return true;
	}
	else if (Cast<UItemDragDropOperation>(InOperation))   //物品Slo槽的拖拽
	{
		UInventorySlotWidget* FSlot = Cast<UItemDragDropOperation>(InOperation)->DraggedSlot;
		if(!FSlot->ItemInfo.bCanBeStacked && FSlot->Amount > 0)
		{
			InventoryWidget->InventoryRef->RemoveItemAtIndex(FSlot->SlotIndex,1);
		}
		else
		{
			ThrowWidget->Update(FSlot->SlotIndex);
			ThrowWidget->SetVisibility(ESlateVisibility::Visible);
			InventoryWidget->InventoryBox->SetIsEnabled(false);
		}
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

void UMainUserWidget::OnInventoryButtonClicked()
{
	if(bInventoryVisibling)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		bInventoryVisibling = false;
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		bInventoryVisibling = true;
	}
}

void UMainUserWidget::AddItemToObtainedQueue(TSubclassOf<ABaseItem> ItemClass, int Amount)
{
	if(!ObtainedItemQueue.IsEmpty())
	{
		ObtainedItemQueue.Enqueue(FInventorySlot{ItemClass,Amount});
	}
	else
	{
		UItemObtainWidget* ObtainWidget = CreateWidget<UItemObtainWidget>(GetWorld(),
			LoadClass<UItemObtainWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/WBP_ItemObtain.WBP_ItemObtain_C'")));
		ObtainWidget->Init(ItemClass,Amount,this);
		ObtainContainer->AddChild(ObtainWidget);
		ObtainedItemQueue.Enqueue(FInventorySlot{ItemClass,Amount});
	}
}

void UMainUserWidget::OnObtainMessageEnd()
{
	ObtainedItemQueue.Pop();
	ObtainContainer->ClearChildren();
	if(!ObtainedItemQueue.IsEmpty())
	{
		UItemObtainWidget* ObtainWidget = CreateWidget<UItemObtainWidget>(GetWorld(),
			LoadClass<UItemObtainWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/WBP_ItemObtain.WBP_ItemObtain_C'")));
		FInventorySlot TempSlot;
		ObtainedItemQueue.Peek(TempSlot);
		ObtainWidget->Init(TempSlot.ItemClass,TempSlot.Amount,this);
		ObtainContainer->AddChild(ObtainWidget);
	}
}

void UMainUserWidget::OnSettingButtonClicked()
{
	if(bSettingVisibling)
	{
		SettingWidget->SetVisibility(ESlateVisibility::Hidden);
		bSettingVisibling = false;
	}
	else
	{
		SettingWidget->SetVisibility(ESlateVisibility::Visible);
		bSettingVisibling = true;
	}
}

void UMainUserWidget::GenerateItemHotKeys(TArray<FKey> ItemKeysToGenerate)
{
	ItemHotKeyWidgets.Empty();
	ItemHotKeyBar->ClearChildren();
	for(FKey Key : ItemKeysToGenerate)
	{
		UItemHotKey* ItemHotKey = CreateWidget<UItemHotKey>(GetWorld(),
			LoadClass<UItemHotKey>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/WBP_ItemHotKey.WBP_ItemHotKey_C'")));
		ItemHotKey->Init(Key,Inventory);
		ItemHotKeyWidgets.Add(ItemHotKey);
		ItemHotKeyBar->AddChild(ItemHotKey);
	}
}

void UMainUserWidget::SkillTreeShow()
{
	SkillTreeComponent->HandleShowCommand();
	UE_LOG(LogTemp, Warning, TEXT("SkillTreeComponent->HandleShowCommand()"));
}

