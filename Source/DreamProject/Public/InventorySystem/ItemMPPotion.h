// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/BaseItem.h"
#include "ItemMPPotion.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API AItemMPPotion : public ABaseItem
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,Category=Data)
	float AddMp;
	virtual void OnUsed() override;
	
};
