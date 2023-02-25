// Fill out your copyright notice in the Description page of Project Settings.


#include "DREAMHUD.h"
#include "ThreadManage.h"
#include "Character/RBaseCharacter.h"
#include "UserWidget/MainUserWidget.h"

void ADREAMHUD::BeginPlay()
{
	Super::BeginPlay();
	//GThread::GetTask().CreateUFunction(BaseCharacter->MainUserWidget->InventoryWidget,TEXT("GenerateSlotWidgets"));
}
