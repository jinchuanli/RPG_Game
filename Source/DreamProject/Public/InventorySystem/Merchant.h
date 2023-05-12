// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/BaseNPC.h"
#include "Merchant.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API AMerchant : public ABaseNPC
{
	GENERATED_BODY()
	friend class UShopWidget;

	class ARBaseCharacter* PlayerCharacter;

	class UShopWidget* ShopWidget;
	
	virtual void BeginPlay() override;
	
	virtual void OnIteractWith(class ARBaseCharacter* Character) override;

	bool bShopOpen;

	virtual void OnLeavePlayerRadius(ARBaseCharacter* Character) override;
public:
	UPROPERTY(EditAnywhere,Category=Item)
	TArray<TSubclassOf<class ABaseItem>> OfferendItems;
};
