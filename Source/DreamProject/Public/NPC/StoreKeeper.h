// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/BaseNPC.h"
#include "StoreKeeper.generated.h"

/**
 * 
 */
UCLASS()
class DREAMPROJECT_API AStoreKeeper : public ABaseNPC
{
	GENERATED_BODY()

	
	class AStorage* StorageRef;

	UPROPERTY(EditAnywhere,Category=Storage)
	TSubclassOf<class AStorage> StorageClass;
	
	virtual void OnIteractWith(class ARBaseCharacter* Character) override;
//	virtual void OnEnterPlayerRadius(ARBaseCharacter* Character) override;
	virtual void OnLeavePlayerRadius(class ARBaseCharacter* Character) override;
	
};
