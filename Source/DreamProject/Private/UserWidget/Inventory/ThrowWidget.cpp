// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/ThrowWidget.h"

#include <algorithm>

#include "Components/TextBlock.h"
#include "InventorySystem/BaseItem.h"
#include "InventorySystem/Inventory.h"
#include "TimerManager.h"
#include "Character/RBaseCharacter.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "Engine/World.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"

#define LOCTEXT_NAMESPACE "Throw"

void UThrowWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MinusButton->OnClicked.AddDynamic(this,&UThrowWidget::OnMinusButtonClicked);
	MinusButton->OnPressed.AddDynamic(this,&UThrowWidget::OnMinusButtonPressed);
	MinusButton->OnReleased.AddDynamic(this,&UThrowWidget::OnMinusButtonReleased);

	PlusButton->OnClicked.AddDynamic(this,&UThrowWidget::OnPlusButtonClicked);
	PlusButton->OnPressed.AddDynamic(this,&UThrowWidget::OnPlusButtonPressed);
	PlusButton->OnReleased.AddDynamic(this,&UThrowWidget::OnPlusButtonReleased);

	ConfirmButton->OnClicked.AddDynamic(this,&UThrowWidget::OnConfirmButtonClicked);
	CancelButton->OnClicked.AddDynamic(this,&UThrowWidget::OnCancelButtonClicked);
}

void UThrowWidget::Update(int Index)
{
	CurrentIndex = Index;
	if(!InventoryRef->bIsSlotEmpty(CurrentIndex))
	{
		ItemInfo = InventoryRef->GetItemAtIndex(CurrentIndex)->ItemInfo;
		MaxAmount = InventoryRef->GetAmountAtIndex(CurrentIndex);
		NameText->SetText(FText::Format(LOCTEXT("Throw","{0} x {1}"),
			ItemInfo.Name,
			MaxAmount));
		ThrowCount = 1;
		CountText->SetText(FText::AsNumber(ThrowCount));
		IconImage->SetBrushFromTexture(ItemInfo.Icon);
	}
}

void UThrowWidget::IncreaseCount()
{
	ThrowCount = FMath::Clamp(ThrowCount + 1,1,MaxAmount);
	CountText->SetText(FText::AsNumber(ThrowCount));
}

void UThrowWidget::DecreaseCount()
{
	ThrowCount = FMath::Clamp(ThrowCount - 1,1,MaxAmount);
	CountText->SetText(FText::AsNumber(ThrowCount));
}

void UThrowWidget::OnMinusButtonClicked()
{
	ClikCount++;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CounttoZero,this,&UThrowWidget::SetClickCountToZero,0.2f,false);

	if(ClikCount > 1)
	{
		ClikCount = 0;
		ThrowCount = 1;
		CountText->SetText(FText::AsNumber(ThrowCount));
	}
	else
	{
		DecreaseCount();	
	}
}

void UThrowWidget::OnPlusButtonClicked()
{
	ClikCount++;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CounttoZero,this,&UThrowWidget::SetClickCountToZero,0.2f,false);

	if(ClikCount > 1)
	{
		ClikCount = 0;
		ThrowCount = MaxAmount;
		CountText->SetText(FText::AsNumber(ThrowCount));
	}
	else
	{
		IncreaseCount();	
	}
}

void UThrowWidget::SetClickCountToZero()
{
	ClikCount = 0;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CounttoZero);
}

void UThrowWidget::OnMinusButtonPressed()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Decrease,this,&UThrowWidget::DecreaseCount,0.1f,true,1.f);
}

void UThrowWidget::OnPlusButtonPressed()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Increase,this,&UThrowWidget::IncreaseCount,0.1f,true,1.f);
}

void UThrowWidget::OnMinusButtonReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Decrease);
}

void UThrowWidget::OnPlusButtonReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Increase);
}

void UThrowWidget::OnConfirmButtonClicked()
{
	if(InventoryRef->RemoveItemAtIndex(CurrentIndex,ThrowCount))
	{
		this->SetVisibility(ESlateVisibility::Hidden);
		InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(true);
	}
}

void UThrowWidget::OnCancelButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(true);
}

#undef LOCTEXT_NAMESPACE
