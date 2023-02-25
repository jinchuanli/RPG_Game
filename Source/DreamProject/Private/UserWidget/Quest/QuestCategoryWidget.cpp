// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Quest/QuestCategoryWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UQuestCategoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	switch (CurrenState)
	{
	case EQuestStates::CurrentQuest:
		SetCategoryName(FText::FromString("Current Quest"));
		break;
	case EQuestStates::CompletedQuest:
		SetCategoryName(FText::FromString("Completed Quest"));
		break;
	case EQuestStates::FailedQuest:
		SetCategoryName(FText::FromString("Failed Quest"));
		break;
	default:
		break;
	}
	ExpandButton->OnClicked.AddDynamic(this,&UQuestCategoryWidget::OnExpandButtonClicked);
	UE_LOG(LogTemp,Warning,TEXT("生成分类成功"));
}

void UQuestCategoryWidget::OnExpandButtonClicked()
{
	bExpand = !bExpand;
	ExpandButton->SetRenderTransformAngle(bExpand ? 0 : -90);
	QuestBox->SetVisibility(bExpand ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	UE_LOG(LogTemp,Warning,TEXT("设置图标旋转"));
}

void UQuestCategoryWidget::SetCategoryName(FText Text)
{
	CategoriesName->SetText(Text);
}
