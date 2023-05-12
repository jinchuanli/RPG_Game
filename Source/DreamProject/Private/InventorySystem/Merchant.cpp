// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Merchant.h"

#include "Character/RBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/ShopWidget.h"
#include "UserWidget/Inventory/SellWidget.h"

void AMerchant::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this,0));

	ShopWidget = CreateWidget<UShopWidget>(GetWorld(),
		LoadClass<UShopWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Inventory/Merchant/WBP_Shop.WBP_Shop_C'")));

	ShopWidget->AddToViewport(3);
	ShopWidget->SetDesiredSizeInViewport(FVector2D(500,700));
	ShopWidget->SetAnchorsInViewport(FAnchors(0.5,0.5,0.5,0.5));
	ShopWidget->SetAlignmentInViewport(FVector2D(0.5,0.5));
	ShopWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMerchant::OnIteractWith(ARBaseCharacter* Character)
{
	if(!bShopOpen)
	{
		ShopWidget->Merchant = this;
		ShopWidget->PlayerCharacter = PlayerCharacter;

		ShopWidget->SellWidget->InventoryRef = PlayerCharacter->InventoryRef;
		ShopWidget->SellWidget->ShopWidget = ShopWidget;
		ShopWidget->UpdateCoin();
		ShopWidget->GenerateItemList();
		ShopWidget->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerCharacter->PC->SetInputMode(InputMode);
		PlayerCharacter->PC->bEnableClickEvents = true;
		PlayerCharacter->PC->bShowMouseCursor = true;
		if(Character->MainUserWidget && !Character->MainUserWidget->ShopWidget)
			Character->MainUserWidget->ShopWidget = ShopWidget;
		bShopOpen = true;
	}
	else
	{
		ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameOnly InputMode;
		PlayerCharacter->PC->bEnableClickEvents = false;
		PlayerCharacter->PC->bShowMouseCursor = false;
		PlayerCharacter->PC->SetInputMode(InputMode);
		bShopOpen = false;
	}
}

void AMerchant::OnLeavePlayerRadius(ARBaseCharacter* Character)
{
	Super::OnLeavePlayerRadius(Character);
	ShopWidget->SetVisibility(ESlateVisibility::Hidden);
	FInputModeGameOnly InputMode;
	PlayerCharacter->PC->bEnableClickEvents = false;
	PlayerCharacter->PC->bShowMouseCursor = false;
	PlayerCharacter->PC->SetInputMode(InputMode);
	bShopOpen = false;
}
