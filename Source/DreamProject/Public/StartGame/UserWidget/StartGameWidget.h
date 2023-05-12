// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartGameWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UStartGameWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UButton* StartGame;
	UPROPERTY(meta=(BindWidget))
	UButton* LoadGame;
	UPROPERTY(meta=(BindWidget))
	UButton* ExitGame;
protected:
	
virtual bool Initialize() override;
	
public:
	UFUNCTION()
	void OpenLevel();

	UFUNCTION()
	void OnClickExitGame();
};
