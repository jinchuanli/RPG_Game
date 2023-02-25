// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillTree/CategoryWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UserWidget/SkillTree/MainTreeWidget.h"

void UCategoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CategoryNameText->SetText(CategoryName);
	CategoryButton->OnHovered.AddDynamic(this,&UCategoryWidget::OnCategoryButtonHoverd);
	CategoryButton->OnUnhovered.AddDynamic(this,&UCategoryWidget::OnCategoryButtUnHoverd);
	CategoryButton->OnClicked.AddDynamic(this,&UCategoryWidget::OnCategoryButtonClicked);
}

void UCategoryWidget::OnCategoryButtonHoverd()
{
	CategoryNameText->SetColorAndOpacity(FLinearColor(0.83,0.83,0.03,1));
}

void UCategoryWidget::OnCategoryButtUnHoverd()
{
	if(!bSelected)
	{
		CategoryNameText->SetColorAndOpacity(FLinearColor(0.4,0.4,0.4,1));
	}
}

void UCategoryWidget::OnCategoryButtonClicked()
{
	MainTreeRef->OnCategoryClicked(Index);
}

void UCategoryWidget::ChangeState(bool FbSelected)
{
	if(FbSelected != this->bSelected)
	{
		this->bSelected = FbSelected;
		FLinearColor TempColor;
		if(FbSelected)
		{
			TempColor = FLinearColor::White;
		}
		else
		{
			TempColor = FLinearColor(0.4,0.4,0.4,1);
		}
		CategoryNameText->SetColorAndOpacity(TempColor);
		CategoryButton->SetIsEnabled(!FbSelected);
	}
}
