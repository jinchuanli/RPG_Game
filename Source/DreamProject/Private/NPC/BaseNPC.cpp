// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/BaseNPC.h"

#include "PaperSpriteComponent.h"
#include "Character/RBaseCharacter.h"
#include "Components/WidgetComponent.h"
#include "Quest/QuestManager.h"
#include "Quest/BaseQuest.h"
#include "UserWidget/Quest/InteractionWidget.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "UserWidget/Quest/MessageWidget.h"

// Sets default values
ABaseNPC::ABaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(RootComponent);
	InteractionWidget->SetVisibility(false);
	// InteractionWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// InteractionWidget->SetCollisionResponseToAllChannels(ECR_Ignore);
	// InteractionWidget->SetGenerateOverlapEvents(false);
	InteractionWidget->SetCollisionProfileName(TEXT("NoCollision"));
	InteractionWidget->SetGenerateOverlapEvents(false);
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> IW(TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Quest/WBP_Interaction.WBP_Interaction_C'"));
	InteractionWidget->SetWidgetClass(IW.Class);
	
	QuestIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("QuestIcon"));
	QuestIcon->SetupAttachment(RootComponent);
	QuestIcon->SetRelativeLocation(FVector(0,0,150.f));
	QuestIcon->SetRelativeScale3D(FVector(0.3,0.3,0.3));
	QuestIcon->SetSpriteColor(FLinearColor::Yellow);
	QuestIcon->SetCollisionProfileName(TEXT("NoCollision"));
	QuestIcon->SetGenerateOverlapEvents(false);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> QuestSprite(TEXT("PaperSprite'/Game/Texture/QuestTextures/Quest_Icon_Sprite.Quest_Icon_Sprite'"));
	QuestIcon->SetSprite(QuestSprite.Object);

	MessageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MessageWidgetComponent"));
	MessageWidgetComponent->SetupAttachment(RootComponent);
	MessageWidgetComponent->SetVisibility(false);
	MessageWidgetComponent->SetCollisionProfileName(TEXT("NoCollision"));
	MessageWidgetComponent->SetGenerateOverlapEvents(false);
	MessageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> MW(TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Quest/WBP_Message.WBP_Message_C'"));
	MessageWidgetComponent->SetWidgetClass(MW.Class);

	GetMesh()->SetRelativeLocation(FVector(0,0,-88));
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
}

// Called when the game starts or when spawned
void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
	Cast<UInteractionWidget>(InteractionWidget->GetUserWidgetObject())->SetNameText(Name);
	QuestIcon->SetVisibility(bHasQuest);
	QuestIcon->SetWorldRotation(FRotator(0,88.5,-90));

	SetOwner(Cast<AActor>(UGameplayStatics::GetPlayerCharacter(this,0)));
	QuestIcon->SetOwnerNoSee(true);

	MessageWidget = Cast<UMessageWidget>(MessageWidgetComponent->GetUserWidgetObject());
}

void ABaseNPC::ResetMessage()
{
	MessageWidgetComponent->SetVisibility(false);
	InteractionWidget->SetVisibility(bInPlayRadius);
	bCanTalkTo = true;
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetMessage);
}

// Called every frame
void ABaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseNPC::OnEnterPlayerRadius(ARBaseCharacter* Character)
{
	bInPlayRadius = true;
	if(bCanTalkTo)
		InteractionWidget->SetVisibility(true);
}

void ABaseNPC::OnLeavePlayerRadius(ARBaseCharacter* Character)
{
	InteractionWidget->SetVisibility(false);
	bInPlayRadius = false;
}

void ABaseNPC::OnIteractWith(ARBaseCharacter* Character)
{
	if(MyQuest)
	{//任务还没有接受时
		if(!Character->QuestManager->AllQuestClasses.Contains(MyQuest))
		{
			Character->QuestManager->AddNewQuest(MyQuest);
			QuestIcon->SetVisibility(false);
		}
		else//任务已经接受
		{
			OnTalkWith(Character);
		}
	}
	else
	{
		OnTalkWith(Character);
	}
}

void ABaseNPC::OnTalkWith(ARBaseCharacter* Character)
{
	ShowMessage(Message,DefaultDuration,Character);
}

void ABaseNPC::ShowMessage(FText FMessage, float Duration, ARBaseCharacter* Character)
{
	if(bCanTalkTo)
	{
		InteractionWidget->SetVisibility(false);
		MessageWidget->SetMessage(FMessage);
		MessageWidgetComponent->SetVisibility(true);
		bCanTalkTo = false;
		Character->QuestManager->OnTalkToNPC(this->GetClass(),NPCId);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetMessage,this,&ABaseNPC::ResetMessage,Duration,false);
	}
}

