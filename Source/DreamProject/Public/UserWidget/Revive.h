// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Revive.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API URevive : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	UButton* Revive;

	UPROPERTY(BlueprintReadWrite,Category=Respawn)
	class ARBaseCharacter* CharacterRef;
	
	virtual bool Initialize() override;

	UFUNCTION()
	void ClickOnRevive();
};
