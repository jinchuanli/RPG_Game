// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/SyntheticSystem.h"

#include "Character/RBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Inventory/CraftingMenuWidget.h"

void ASyntheticSystem::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	//CraftingMenuWidget = PlayerCharacter->MainUserWidget->CraftingMenu;
}

void ASyntheticSystem::OnIteractWith(ARBaseCharacter* Character)
{
	PlayerCharacter->MainUserWidget->CraftingMenu->SetVisibility(ESlateVisibility::Visible);
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerCharacter->PC->SetInputMode(InputMode);
	PlayerCharacter->PC->bEnableClickEvents = true;
	PlayerCharacter->PC->bShowMouseCursor = true;
}

void ASyntheticSystem::OnLeavePlayerRadius(ARBaseCharacter* Character)
{
	Super::OnLeavePlayerRadius(Character);
	PlayerCharacter->MainUserWidget->CraftingMenu->SetVisibility(ESlateVisibility::Hidden);
	FInputModeGameOnly InputMode;
	PlayerCharacter->PC->bEnableClickEvents = false;
	PlayerCharacter->PC->bShowMouseCursor = false;
	PlayerCharacter->PC->SetInputMode(InputMode);
}
