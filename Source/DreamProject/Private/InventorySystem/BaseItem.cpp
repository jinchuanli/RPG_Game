// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/BaseItem.h"

#include "Character/RBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "InventorySystem/Inventory.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget/MainUserWidget.h"
#include "UserWidget/Quest/InteractionWidget.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//开始实例化
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetGenerateOverlapEvents(false);
	RootComponent = StaticMeshComp;
	BoxCollisonComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereCollisonComp"));
	BoxCollisonComp->SetupAttachment(RootComponent);


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

	PlayerRef = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
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
	int RestAmount = Character->InventoryRef->AddItem(this->GetClass(),Amount);
	if(Amount != RestAmount)
	{
		Character->MainUserWidget->AddItemToObtainedQueue(this->GetClass(),Amount-RestAmount);
	}
	if(RestAmount > 0)
	{
		Amount = RestAmount;
		if(ID != -1)
		{
			Character->InventoryRef->LootedPickups.Add(FSavedPickup{ID,RestAmount});
		}
	}
	else
	{
		if(ID != -1)
		{
			Character->InventoryRef->LootedPickups.Add(FSavedPickup{ID,0});
		}
		Destroy();
	}
}

void ABaseItem::OnUsed()
{
	if(InventoryRef->RemoveItemAtIndex(Index,1))
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("你已经使用 %s"),*ItemInfo.Name.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("使用 %s 失败"),*ItemInfo.Name.ToString());
	}
}

