// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillHotKey.h"
#include "InventorySystem/BaseItem.h"
#include "MainUserWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UButton;
class USkillTreeComponent;
class ARBaseCharacter;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

	UTextBlock* LevelText;
	UProgressBar* HPPProgressBar;
	UProgressBar* MPPProgressBar;

	UPROPERTY(Meta = (BindWidget))
	class UVerticalBox* HotkeyRowContainer;

	//
	TArray<class UHotKeyRow*> HotKeyRows;
	//

	
public:
	//ARBaseCharacter* Playref;
	USkillTreeComponent* SkillTreeComponent;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FKey> Keys;
	UPROPERTY(BlueprintReadOnly)
	TArray<USkillHotKey*> AllHotkeySlots;

	//绑定
	virtual bool Initialize() override;
	void SetLevelText(FText Text);
	void SetHPPProgressBar(float Percent);
	void SetMPPProgressBar(float Percent);

	void GenerateHotKeys(TArray<FKey> KeysToGenerate,int32 KeysPerRow);

	FORCEINLINE TArray<USkillHotKey*> GetAllHotkeySlots(){return AllHotkeySlots;}

	//在蓝图中调用c++的方法
	UFUNCTION(BlueprintNativeEvent,Category=Animation)
	void PlayPopUpAnimation();
	void PlayPopUpAnimation_Implementation();   //NativeEvent在南图中没有写实现的话，自动调用c++中的函数

	//
  //技能丢弃，实际上是丢到主界面上面,主界面的丢弃函数
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )override;


	//128
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* EnemyNameLevel;
	UPROPERTY(Meta = (BindWidget))
	UProgressBar* EnemyHpBar;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* EnemyHpText;
	UPROPERTY(Meta = (BindWidget))
	class UBorder* EnemyBorder;

	//145
	UPROPERTY(Meta = (BindWidget))
	class UHorizontalBox* BuffBox;

///////////////////////////////////////////////////////////////////////////	//经验
private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* CurrentExpText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* NeedExpToNextLevelText;
	UPROPERTY(Meta = (BindWidget))
	UProgressBar* ExpBar;
public:
	void SetExpPanel(float CurrentExp,float NeedExpToNextLevel);



	/////////////////////////////////////////////////////////SkillTree
	UPROPERTY(Meta = (BindWidget))
	class UMainTreeWidget* SkillTree;




	///////////////////////////////////////////////////////////////////////quest
	class AQuestManager* QuestManager;

	UPROPERTY(Meta = (BindWidget))
	class USizeBox* QuestListSizeBox;
	UPROPERTY(Meta = (BindWidget))
	class UScrollBox* QuestList;

	class UQuestWidget* AddQuestToList(class ABaseQuest* Quest);

	UPROPERTY(BlueprintReadOnly)
	TArray<class UQuestWidget*>QuestWidgets;

	UPROPERTY(BlueprintReadWrite)
	bool bOutSide = true;

	UPROPERTY(Meta = (BindWidget))
	class UMiniMapWidget* MiniMap;

	UPROPERTY(Meta = (BindWidget))
	class UQuestJournal* QuestJournal;

	UPROPERTY(Meta = (BindWidget))
	UButton* Button_Quest;

	UPROPERTY(Meta = (BindWidget))
	UButton* Button_Skill;



	UFUNCTION()
	void OnQuestButtonClicked();

	UFUNCTION()
	void OnSkillButtonClicked();

	bool bTreeShown = false;
	
	bool bQuestVisibling = false;


	////////////////////////////////////////////////////////////////////////////////////inventory
protected:
	UPROPERTY(Meta = (BindWidget))
	UButton* Button_Inventory;
public:
	bool bInventoryVisibling = false;
	
	UPROPERTY(meta=(BindWidget))
	class UInventoryWidget* InventoryWidget;

	UFUNCTION()
	void OnInventoryButtonClicked();

	UPROPERTY(meta=(BindWidget))
	class UThrowWidget* ThrowWidget;

	UPROPERTY(meta=(BindWidget))
	class UBorder* ObtainContainer;
	UPROPERTY(meta=(BindWidget))
	UBorder* ReadableContainer;
	

	//用队列实现一次性界面上只出现一个播放动画
	TQueue<FInventorySlot> ObtainedItemQueue;
	void AddItemToObtainedQueue(TSubclassOf<class ABaseItem> ItemClass,int Amount);
	UFUNCTION(BlueprintCallable)
	void OnObtainMessageEnd();

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	class UCraftingMenuWidget* CraftingMenu;
	
	class UShopWidget* ShopWidget;


	////////////////////////////////////////////////////////save
	UPROPERTY(meta=(BindWidget))
	UButton* Button_Setting;
	
	UPROPERTY(meta=(BindWidget))
	class USettingWidget* SettingWidget;

	bool bSettingVisibling = false;
	UFUNCTION()
	void OnSettingButtonClicked();

	/////////////////////////////////////////////////////////////////////////storage
	UPROPERTY(meta=(BindWidget))
	class UStorageWidget* StorageWidget;
	
	////////////////////////////////////////////////////////////////////////////////
	class AInventory* Inventory;
	
	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* ItemHotKeyBar;

	TArray<class UItemHotKey*> ItemHotKeyWidgets;
	
	void GenerateItemHotKeys(TArray<FKey> ItemKeysToGenerate);

	UFUNCTION()
	void SkillTreeShow();
};
