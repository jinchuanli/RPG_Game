// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/InterationInterface.h"
#include "BaseNPC.generated.h"

UCLASS()
class DREAMPROJECT_API ABaseNPC : public ACharacter,public IInterationInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNPC();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* InteractionWidget;

	UPROPERTY(EditAnywhere,Category=Quest)
	FText Name;

	UPROPERTY(EditAnywhere,Category=Quest)  //在南图中指定需要用到TSubclassOf
	TSubclassOf<class ABaseQuest> MyQuest;

	UPROPERTY(VisibleAnywhere)
	class UPaperSpriteComponent* QuestIcon;

	UPROPERTY(EditAnywhere,Category=Quest)
	class UMessageWidget* MessageWidget;
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* MessageWidgetComponent;
	
	UPROPERTY(VisibleAnywhere,Category=Quest)
	bool bInPlayRadius;

	FTimerHandle TimerHandle_ResetMessage;
	void ResetMessage();
public:
	UPROPERTY(EditAnywhere,Category=Quest)
	bool bHasQuest;
	
	UPROPERTY(EditAnywhere,Category=Quest)
	int NPCId;  //NPCId进行表示判断是否执行任务的NPC
	UPROPERTY(EditAnywhere,Category=Quest)
	bool bCanTalkTo;
	UPROPERTY(EditAnywhere,Category=Quest)
	FText Message;

	//时间间隔
	UPROPERTY(EditAnywhere,Category=Quest)
	float DefaultDuration = 5.f;
	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnEnterPlayerRadius(class ARBaseCharacter* Character) override;
	virtual void OnLeavePlayerRadius(class ARBaseCharacter* Character) override;
	virtual void OnIteractWith(class ARBaseCharacter* Character) override;

	//显示交谈的内容
	void OnTalkWith(class ARBaseCharacter* Character);
	void ShowMessage(FText Message,float Duration,class ARBaseCharacter* Character);
};
