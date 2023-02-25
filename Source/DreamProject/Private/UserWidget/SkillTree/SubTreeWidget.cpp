// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/SkillTree/SubTreeWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UserWidget/SkillTree/ConnectionWidget.h"
#include "UserWidget/SkillTree/SkillTreeEntryWidget.h"

void USubTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GenerateContent();
}

void USubTreeWidget::GenerateContent()
{
	Canvas->ClearChildren();
	SkillEntries.Empty();
	Connections.Empty();

	for(FSubTreeEntry Entry : Content.Skills)
	{
		USkillTreeEntryWidget* SkillTreeEntryWidget = CreateWidget<USkillTreeEntryWidget>(GetWorld(),
			LoadClass<USkillTreeEntryWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/SkillTree/WBP_SkillTreeEntry.WBP_SkillTreeEntry_C'")));
		SkillTreeEntryWidget->SubTree = this;
		SkillTreeEntryWidget->SkillClass = Entry.SpellClass;
		SkillEntries.Add(SkillTreeEntryWidget);
		UCanvasPanelSlot* TempSlot = Canvas->AddChildToCanvas(SkillTreeEntryWidget);
		TempSlot->SetPosition(Entry.Position);
		TempSlot->SetZOrder(2);
		TempSlot->SetAutoSize(true);
	}

	for(FSubTreeConnection Connection : Content.Connections)
	{
		UConnectionWidget* ConnectionWidget = CreateWidget<UConnectionWidget>(GetWorld(),
			LoadClass<UConnectionWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/SkillTree/WBP_Connection.WBP_Connection_C'")));
		ConnectionWidget->ForSpell = Connection.ForSpell;
		Connections.Add(ConnectionWidget);
		UCanvasPanelSlot* TempSlot = Canvas->AddChildToCanvas(ConnectionWidget);
		TempSlot->SetPosition(Connection.Position);
		TempSlot->SetSize(Connection.size);
		ConnectionWidget->SetRenderTransform(Connection.Transform);
	}
}
