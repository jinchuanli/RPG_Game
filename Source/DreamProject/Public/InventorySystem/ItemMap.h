// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/BaseItem.h"
#include "ItemMap.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API AItemMap : public ABaseItem
{
	GENERATED_BODY()

	
public:
	virtual void OnUsed() override;
	void OnReadOver();
	class UReadableItemWidget* Map;
};
