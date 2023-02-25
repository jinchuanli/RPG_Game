// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/Props/BaseQuestProp.h"

#include "Character/RBaseCharacter.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Quest/QuestManager.h"
#include "UserWidget/Quest/InteractionWidget.h"

// Sets default values
ABaseQuestProp::ABaseQuestProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(RootComponent);
//设置初始的值
	InteractionWidget->SetRelativeScale3D(FVector(0.1,0.1,0.1));
	InteractionWidget->SetRelativeRotation(FRotator(0,90,0));
	InteractionWidget->SetCollisionProfileName(TEXT("NoCollision"));
	InteractionWidget->SetGenerateOverlapEvents(false);
	InteractionWidget->SetVisibility(false);
	static ConstructorHelpers::FClassFinder<UUserWidget> US(TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Quest/WBP_Interaction.WBP_Interaction_C'"));
	InteractionWidget->SetWidgetClass(US.Class);

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	ParticleSystemComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseQuestProp::BeginPlay()
{
	Super::BeginPlay();
	Cast<UInteractionWidget>(InteractionWidget->GetUserWidgetObject())->NameText->SetText(Name);
	Cast<UInteractionWidget>(InteractionWidget->GetUserWidgetObject())->InteractionText->SetText(FText::FromString("Press [F] to pick up"));
}

// Called every frame
void ABaseQuestProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseQuestProp::OnEnterPlayerRadius(ARBaseCharacter* Character)
{
	InteractionWidget->SetVisibility(true);
}

void ABaseQuestProp::OnLeavePlayerRadius(ARBaseCharacter* Character)
{
	InteractionWidget->SetVisibility(false);
}

void ABaseQuestProp::OnIteractWith(ARBaseCharacter* Character)
{
	Character->ObtainedProps.Add(this->GetClass());
	Character->QuestManager->OnObjectFound(this->GetClass());
	Destroy();
}

