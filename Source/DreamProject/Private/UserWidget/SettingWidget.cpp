// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SettingWidget.h"

#include "Character/RBaseCharacter.h"
#include "Components/Button.h"
#include "InventorySystem/Inventory.h"
#include "InventorySystem/Storage.h"
#include "Kismet/GameplayStatics.h"

void USettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SaveButton->OnClicked.AddDynamic(this,&USettingWidget::OnSaveButtonClicked);
}

void USettingWidget::OnSaveButtonClicked()
{
	if(!Playercharacter)
	{
		Playercharacter = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	}
	Playercharacter->SaveGame();
	if(Playercharacter && Playercharacter->InventoryRef)
	{
		Playercharacter->InventoryRef->SaveInventory();
	}
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AStorage::StaticClass(),FoundActors);
	for(AActor* Actor : FoundActors)
	{
		AStorage* Storage = Cast<AStorage>(Actor);
		if(Storage)
		{
			Storage->SaveStorage();
		}
	}
}
