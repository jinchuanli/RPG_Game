// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/BaseItem.h"
#include "ItemHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API AItemHealthPotion : public ABaseItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category=Data)
	float AddHp;
	virtual void OnUsed() override;
	
};
