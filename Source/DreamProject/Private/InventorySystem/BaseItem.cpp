// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/BaseItem.h"

#include "Character/RBaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "InventorySystem/Inventory.h"
#include "UserWidget/Quest/InteractionWidget.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//开始实例化
	SphereCollisonComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisonComp"));
	RootComponent = SphereCollisonComp;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);

	InterationWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InterationWidgetComp"));
	InterationWidgetComp->SetupAttachment(RootComponent);
	InterationWidgetComp->SetRelativeScale3D(FVector(0.7,0.7,0.7));
	InterationWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	InterationWidgetComp->SetCollisionProfileName(TEXT("NoCollision"));
	InterationWidgetComp->SetGenerateOverlapEvents(false);
	InterationWidgetComp->SetVisibility(false);
	static ConstructorHelpers::FClassFinder<UUserWidget> US(TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Quest/WBP_Interaction.WBP_Interaction_C'"));
	InterationWidgetComp->SetWidgetClass(US.Class);
	
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	//设置交互显示的内容
	Cast<UInteractionWidget>(InterationWidgetComp->GetUserWidgetObject())->NameText->SetText(ItemInfo.Name);
	Cast<UInteractionWidget>(InterationWidgetComp->GetUserWidgetObject())->InteractionText->SetText(FText::FromString("Press [F] to pick up!"));
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseItem::OnEnterPlayerRadius(ARBaseCharacter* Character)
{
	InterationWidgetComp->SetVisibility(true);
}

void ABaseItem::OnLeavePlayerRadius(ARBaseCharacter* Character)
{
	InterationWidgetComp->SetVisibility(false);
}

void ABaseItem::OnIteractWith(ARBaseCharacter* Character)
{
	//按e键进行交互然后摧毁
	if(Character->InventoryRef->AddItem(this->GetClass(),Amount) > 0)
	{
		Amount = Character->InventoryRef->AddItem(this->GetClass(),Amount);
	}
	else
	{
		Destroy();
	}
}

