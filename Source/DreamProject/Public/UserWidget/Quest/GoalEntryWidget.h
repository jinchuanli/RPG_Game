// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quest/QuestStruct.h"
#include "GoalEntryWidget.generated.h"

class UTexture2D;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UGoalEntryWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	class UImage* GoalStateImage;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* GoalText;



	UPROPERTY(EditAnywhere,Category="StateIcon")
	UTexture2D* CurrentTexture;
	UPROPERTY(EditAnywhere,Category="StateIcon")
	UTexture2D* SuccessTexture;
	UPROPERTY(EditAnywhere,Category="StateIcon")
	UTexture2D* FailedTexture;

	void Update();  //文字的更新

	virtual void NativeConstruct() override;
public:
	FGoalInfo GoalInfo;
	EGoalStates GoalState;
	class UQuestJournal* Journal;
	void SetGoalText(FText Text);

	int HuntIndex;
};
