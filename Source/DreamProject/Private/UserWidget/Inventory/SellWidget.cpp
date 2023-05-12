// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/Inventory/SellWidget.h"
#include <algorithm>
#include "Components/TextBlock.h"
#include "InventorySystem/BaseItem.h"
#include "InventorySystem/Inventory.h"
#include "TimerManager.h"
#include "Character/RBaseCharacter.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "Components/WrapBox.h"
#include "Engine/World.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UserWidget/Inventory/ShopWidget.h"

#define LOCTEXT_NAMESPACE "Throw"

void USellWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MinusButton->OnClicked.AddDynamic(this,&USellWidget::OnMinusButtonClicked);
	MinusButton->OnPressed.AddDynamic(this,&USellWidget::OnMinusButtonPressed);
	MinusButton->OnReleased.AddDynamic(this,&USellWidget::OnMinusButtonReleased);

	PlusButton->OnClicked.AddDynamic(this,&USellWidget::OnPlusButtonClicked);
	PlusButton->OnPressed.AddDynamic(this,&USellWidget::OnPlusButtonPressed);
	PlusButton->OnReleased.AddDynamic(this,&USellWidget::OnPlusButtonReleased);

	ConfirmButton->OnClicked.AddDynamic(this,&USellWidget::OnConfirmButtonClicked);
	CancelButton->OnClicked.AddDynamic(this,&USellWidget::OnCancelButtonClicked);
}

void USellWidget::Update(int Index)
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
	UpdatePrice();
}

void USellWidget::IncreaseCount()
{
	ThrowCount = FMath::Clamp(ThrowCount + 1,1,MaxAmount);
	CountText->SetText(FText::AsNumber(ThrowCount));
	UpdatePrice();
}

void USellWidget::DecreaseCount()
{
	ThrowCount = FMath::Clamp(ThrowCount - 1,1,MaxAmount);
	CountText->SetText(FText::AsNumber(ThrowCount));
	UpdatePrice();
}

void USellWidget::OnMinusButtonClicked()
{
	ClikCount++;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CounttoZero,this,&USellWidget::SetClickCountToZero,0.2f,false);

	if(ClikCount > 1)
	{
		ClikCount = 0;
		ThrowCount = 1;
		CountText->SetText(FText::AsNumber(ThrowCount));
		UpdatePrice();
	}
	else
	{
		DecreaseCount();	
	}
}

void USellWidget::OnPlusButtonClicked()
{
	ClikCount++;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CounttoZero,this,&USellWidget::SetClickCountToZero,0.2f,false);

	if(ClikCount > 1)
	{
		ClikCount = 0;
		ThrowCount = MaxAmount;
		CountText->SetText(FText::AsNumber(ThrowCount));
		UpdatePrice();
	}
	else
	{
		IncreaseCount();	
	}
}

void USellWidget::SetClickCountToZero()
{
	ClikCount = 0;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CounttoZero);
}

void USellWidget::OnMinusButtonPressed()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Decrease,this,&USellWidget::DecreaseCount,0.1f,true,1.f);
}

void USellWidget::OnPlusButtonPressed()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Increase,this,&USellWidget::IncreaseCount,0.1f,true,1.f);
}

void USellWidget::OnMinusButtonReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Decrease);
}

void USellWidget::OnPlusButtonReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Increase);
}

void USellWidget::OnConfirmButtonClicked()
{
	if(InventoryRef->RemoveItemAtIndex(CurrentIndex,ThrowCount))
	{
		InventoryRef->PlayerCharacter->IncreaseCoin(ThrowCount*ItemInfo.Price);
		this->SetVisibility(ESlateVisibility::Hidden);
		InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(true);
		ShopWidget->GridPanel->SetIsEnabled(true);
		ShopWidget->UpdateAllItems();
	}
}

void USellWidget::OnCancelButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	InventoryRef->PlayerCharacter->MainUserWidget->InventoryWidget->InventoryBox->SetIsEnabled(true);
	ShopWidget->GridPanel->SetIsEnabled(true);
}

void USellWidget::UpdatePrice()
{
	PriceText->SetText(FText::AsNumber(ItemInfo.Price * ThrowCount));
}

#undef LOCTEXT_NAMESPACE
