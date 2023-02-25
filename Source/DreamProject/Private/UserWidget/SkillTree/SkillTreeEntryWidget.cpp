// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillTree/SkillTreeEntryWidget.h"
#include "Character/Skill/BaseSkill.h"
#include "Character/RBaseCharacter.h"
#include "Components/Image.h"
#include "Components/SkillTreeComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UserWidget/SkillTree/MainTreeWidget.h"
#include "UserWidget/SkillTree/SubTreeWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UserWidget/SkillDeag.h"
#include "UserWidget/SkillDragOperation.h"

#define LOCTEXT_NAMESPACE "SkillEntryWidget"

void USkillTreeEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillActor = GetWorld()->SpawnActor<ABaseSkill>(SkillClass,FVector::ZeroVector,FRotator::ZeroRotator);
	SkillActor->SetPlayerReference(SubTree->MainTree->SkillTreeComp->PlayerCharacter);
	UpdateStageText();
	UpdateIcon();
	UpdateUpgradeBox();
	
	//注册按钮
	PlusButton->OnClicked.AddDynamic(this,&USkillTreeEntryWidget::OnPlusButtonClicked);
	MinusButton->OnClicked.AddDynamic(this,&USkillTreeEntryWidget::OnMinusButtonClicked);
}

int USkillTreeEntryWidget::GetAmountOfStages()
{
	return SkillActor->GetSkillInfo().Stages.Num();
}

void USkillTreeEntryWidget::UpdateStageText()
{
	StageText->SetText(FText::Format(LOCTEXT("SkillEntryWidget","{0}/{1}"),SkillActor->GetCurrentStageIndex()+1,
		FText::AsNumber(GetAmountOfStages())));
}

void USkillTreeEntryWidget::UpdateIcon()
{
	UTexture2D* TempTexture;
	if(SkillActor->GetCurrentStageIndex() < 0)
	{
		TempTexture = SkillActor->GetSkillInfo().Icon;
	}
	else
	{
		if(SkillActor->GetCurrentStage().OverrideIcon)
		{
			TempTexture = SkillActor->GetCurrentStage().OverrideIcon;
		}
		else
		{
			TempTexture = SkillActor->GetSkillInfo().Icon;
		}
	}
	SkillIcon->SetBrushFromTexture(TempTexture);
}

void USkillTreeEntryWidget::UpdateUpgradeBox()
{
	if(SubTree->MainTree->SkillTreeComp->BCanUpgradeSpell(SkillActor))
	{
		PlusButton->SetVisibility(ESlateVisibility::Visible);
		MinusButton->SetVisibility(ESlateVisibility::Hidden);
		UpgradeBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);  //可见但是不能点击
		if(SkillActor->GetCurrentStageIndex() >= 1)
		{
			MinusButton->SetVisibility(ESlateVisibility::Visible);
		}
		SetColorAndOpacity(FLinearColor::White);
	}
	else
	{
		PlusButton->SetVisibility(ESlateVisibility::Hidden);
		if(SkillActor->GetCurrentStageIndex() >= 1)
		{
			MinusButton->SetVisibility(ESlateVisibility::Visible);
			UpgradeBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);   //可见但是不能点击
			SetColorAndOpacity(FLinearColor::White);
		}
		else
		{
			if(SubTree->MainTree->SkillTreeComp->BPlayerLearnedSpell(SkillClass))
			{
				SetColorAndOpacity(FLinearColor::White);
				UpgradeBox->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				SetColorAndOpacity(FLinearColor(1,1,1,0.3f));
				UpgradeBox->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void USkillTreeEntryWidget::OnSpellLearnt()
{
	bSpellLearnt = true;
	SkillIcon->SetColorAndOpacity(FLinearColor::White);
}

void USkillTreeEntryWidget::OnPlusButtonClicked()
{
	SubTree->MainTree->SkillTreeComp->UpgradeSpell(SkillActor,this);
}

void USkillTreeEntryWidget::OnMinusButtonClicked()
{
	SubTree->MainTree->SkillTreeComp->DowngradeSpell(SkillActor,this);
}

FReply USkillTreeEntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent); //调用父类的功能
	
	if(bSpellLearnt)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::RightMouseButton).NativeReply;
	}
	else
	{
		return FReply::Handled();
	}
}

void USkillTreeEntryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                 UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if(!SubTree->MainTree->SkillTreeComp->BSkillOnHotkey(SkillActor))
	{
		//创建拖拽的图标
		USkillDeag* SkillDrag = CreateWidget<USkillDeag>(GetWorld(),LoadClass<USkillDeag>(GetWorld(),
			TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/WBP_SkillDrag.WBP_SkillDrag_c'")));
		//附上图标  OverrideIcon升级过后的图标
		if(SkillActor->GetCurrentStage().OverrideIcon)
		{
			SkillDrag->SetSkillTexture(SkillActor->GetCurrentStage().OverrideIcon);
		}
		else
		{
			SkillDrag -> SetSkillTexture(SkillActor->GetSkillInfo().Icon);
		}
		//创建一个可以拖拽的操作
		UDragDropOperation* TempOp = UWidgetBlueprintLibrary::CreateDragDropOperation(SkillDragOperation);
		TempOp->DefaultDragVisual = SkillDrag;
		OutOperation = TempOp;

		if(Cast<USkillDragOperation>(OutOperation))
		{
			Cast<USkillDragOperation>(OutOperation)->SkillActor = SkillActor;
			Cast<USkillDragOperation>(OutOperation)->FromHotKey = nullptr;
		}
	}
}

#undef LOCTEXT_NAMESPACE
