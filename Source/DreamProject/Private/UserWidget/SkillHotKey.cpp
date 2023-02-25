// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillHotKey.h"
#include "Character/Skill/BaseSkill.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UserWidget/SkillDeag.h"
#include "UserWidget/SkillDragOperation.h"

bool USkillHotKey::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	
	//绑定技能
	SkillButton->OnClicked.AddDynamic(this,&USkillHotKey::OnSkillButtonClicked);
	CooldownMat = CoolDownImage->GetDynamicMaterial();//获得材质实例
	DragOverColor = FLinearColor(1.f,0.82f,0.0f,0.5f);
	DefaultColor = FLinearColor(0,0,0,0.2f);
	return true;
}

void USkillHotKey::OnSkillButtonClicked()
{
	AssignedSpell->OnTryCastSpell();
}

void USkillHotKey::ResetStyle()
{
	bDraggedOver = false;
	Base->SetColorAndOpacity(DefaultColor);
}

FReply USkillHotKey::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent); //调用父类的功能
	
	if(AssignedSpell)
	{
		if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && !AssignedSpell->GetBOnCoolDown())
		{
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::RightMouseButton).NativeReply;
		}
		else
		{
			return FReply::Handled();
		}
	}
	else
	{
		return FReply::Handled();
	}
}

void USkillHotKey::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	//创建拖拽的图标
	USkillDeag* SkillDrag = CreateWidget<USkillDeag>(GetWorld(),LoadClass<USkillDeag>(GetWorld(),
		TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/WBP_SkillDrag.WBP_SkillDrag_c'")));
	//附上图标  OverrideIcon升级过后的图标
	if(AssignedSpell->GetCurrentStage().OverrideIcon)
	{
		SkillDrag->SetSkillTexture(AssignedSpell->GetCurrentStage().OverrideIcon);
	}
	else
	{
		SkillDrag -> SetSkillTexture(AssignedSpell->GetSkillInfo().Icon);
	}
	//创建一个可以拖拽的操作
	UDragDropOperation* TempOp = UWidgetBlueprintLibrary::CreateDragDropOperation(SkillDragOperation);
	TempOp->DefaultDragVisual = SkillDrag;
	OutOperation = TempOp;

	if(Cast<USkillDragOperation>(OutOperation))
	{
		Cast<USkillDragOperation>(OutOperation)->SkillActor = AssignedSpell;
		Cast<USkillDragOperation>(OutOperation)->FromHotKey = this;
	}
}

bool USkillHotKey::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{//被放上去拖拽物的时候
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	//InOperation是之前从别的地方拽出来的OutOperation
	if(Cast<USkillDragOperation>(InOperation))
	{
		if(!bDraggedOver && Cast<USkillDragOperation>(InOperation)->FromHotKey != this && AssignedSpell == nullptr)
		{
			//背景放上被拖拽的图标
			Base->SetColorAndOpacity(DragOverColor);
			bDraggedOver = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void USkillHotKey::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	if(bDraggedOver)
	{
		//因为OnLeave函数并不是仅仅是在拖拽到另外一个插槽上又离开的时候调用的，所以需要排除一下从当前插槽离开的时候的情况
		if(Cast<USkillDragOperation>(InOperation))
		{
			ResetStyle();
		}
	}
}

bool USkillHotKey::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if(Cast<USkillDragOperation>(InOperation))
	{
		if(Cast<USkillDragOperation>(InOperation)->FromHotKey != this)
		{
			if(AssignedSpell)
			{
				if(AssignedSpell->GetBOnCoolDown())
				{
					return true;
				}
				else //交换技能
				{
					ABaseSkill* LocalAssignedSpell = AssignedSpell;
					//清除分配的技能
					ClearAssignedSpell();
					if(Cast<USkillDragOperation>(InOperation)->FromHotKey)
					{
						Cast<USkillDragOperation>(InOperation)->FromHotKey->ClearAssignedSpell();
						//
						//开始赋值
						AssignSpell(Cast<USkillDragOperation>(InOperation)->SkillActor);
						Cast<USkillDragOperation>(InOperation)->FromHotKey->AssignSpell(LocalAssignedSpell);
					}
					else
					{
						AssignSpell(Cast<USkillDragOperation>(InOperation)->SkillActor);
					}
					ResetStyle();
					return true;
				}
			}
			else
			{
				if(Cast<USkillDragOperation>(InOperation)->FromHotKey)
					Cast<USkillDragOperation>(InOperation)->FromHotKey->ClearAssignedSpell();
				//分配技能
				AssignSpell(Cast<USkillDragOperation>(InOperation)->SkillActor);
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void USkillHotKey::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(!bHasSetKeyName)
	{
		HotKeyName->SetText(Key.GetDisplayName());
		bHasSetKeyName = true;
	}
}

void USkillHotKey::AssignSpell(ABaseSkill* NewAssignSpell)
{
	this->AssignedSpell = NewAssignSpell;
	AssignedSpell->SethotKey(this);
	SkillButton->SetIsEnabled(true);
	if(AssignedSpell->GetCurrentStage().OverrideIcon)
	{
		SkillIcon->SetBrushFromTexture(AssignedSpell->GetCurrentStage().OverrideIcon);
	}
	else
	{
		SkillIcon->SetBrushFromTexture(AssignedSpell->GetSkillInfo().Icon);
	}
	SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void USkillHotKey::ClearAssignedSpell()
{
	if(AssignedSpell)
	{
		AssignedSpell->SethotKey(nullptr);
		AssignedSpell = nullptr;

		SkillButton->SetIsEnabled(false);
		SkillIcon->SetBrushFromTexture(nullptr);
		SkillIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USkillHotKey::EnableHotKey()
{
	bDeactive = false;
	if(!AssignedSpell->GetBOnCoolDown())
	{
		SkillButton->SetIsEnabled(true);
		SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CoolDownImage->SetVisibility(ESlateVisibility::Hidden);
		SkillIcon->SetColorAndOpacity(FLinearColor::White);
	}
}

void USkillHotKey::DisableHotKey()
{
	bDeactive = true;
	if(!AssignedSpell->GetBOnCoolDown())
	{
		SkillButton->SetIsEnabled(false);
		SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CoolDownImage->SetVisibility(ESlateVisibility::Visible);
		CooldownMat->SetScalarParameterValue(FName("Percent"),1.f);
	}
}
