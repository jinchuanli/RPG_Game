// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillTree/MainTreeWidget.h"

#include "Character/RBaseCharacter.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/SkillTreeComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UserWidget/SkillTree/CategoryWidget.h"
#include "UserWidget/SkillTree/SubTreeWidget.h"

void UMainTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this,&UMainTreeWidget::OnCloseButtonClicked);
}

void UMainTreeWidget::OnCategoryClicked(int Index)
{
	if(CurrentlySelectedIndex != Index)
	{
		if(CurrentlySelectedIndex == -1)
		{
			SubTreeSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);   //显示但是不能够点击
			CurrentlySelectedIndex = Index;

			CategoryWidgets[CurrentlySelectedIndex]->ChangeState(true);
			SubTreeSwitcher->SetActiveWidgetIndex(CurrentlySelectedIndex);
		}
		else
		{
			CategoryWidgets[CurrentlySelectedIndex]->ChangeState(false);
			CurrentlySelectedIndex = Index;
			CategoryWidgets[CurrentlySelectedIndex]->ChangeState(true);
			SubTreeSwitcher->SetActiveWidgetIndex(CurrentlySelectedIndex);
		}
	}
}

void UMainTreeWidget::OnCloseButtonClicked()
{
	SkillTreeComp->HandleShowCommand();
}

void UMainTreeWidget::UpdateLevel()
{
	LevelText->SetText(FText::AsNumber(SkillTreeComp->PlayerCharacter->GetCurrentLevel()));
}

void UMainTreeWidget::UpdateSp()
{
	SpText->SetText(FText::AsNumber(SkillTreeComp->SkillPoints));
}

void UMainTreeWidget::GenerateCategories()
{
	CategoriesBox->ClearChildren();
	SubTreeSwitcher->ClearChildren();

	CategoryWidgets.Empty();  //数组清空
	SubTreeWidgets.Empty();

	//添加
	for(int i = 0;i < TreeCategories.Num();i++)
	{
		UCategoryWidget* CategoryWidget = CreateWidget<UCategoryWidget>(GetWorld(),
			LoadClass<UCategoryWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/SkillTree/WBP_Category.WBP_Category_C'")));
		CategoryWidget->CategoryName = TreeCategories[i].Name;
		CategoryWidget->Index = i;
		CategoryWidget->MainTreeRef = this;
		CategoryWidgets.Add(CategoryWidget);

		CategoriesBox->AddChild(CategoryWidget);

		USubTreeWidget* SubTreeWidget = CreateWidget<USubTreeWidget>(GetWorld(),
			LoadClass<USubTreeWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/SkillTree/WBP_SubTree.WBP_SubTree_C'")));

		SubTreeWidget->Content = TreeCategories[i].Content;
		SubTreeWidget->MainTree = this;
		SubTreeWidgets.Add(SubTreeWidget);

		SubTreeSwitcher->AddChild(SubTreeWidget);
	}
}

void UMainTreeWidget::Initialze(USkillTreeComponent* AssignedTreeComp)
{
	SkillTreeComp = AssignedTreeComp;
	TreeCategories = SkillTreeComp->Categories;
	UpdateLevel();
	UpdateSp();
	GenerateCategories();
	OnCategoryClicked(0);
}
