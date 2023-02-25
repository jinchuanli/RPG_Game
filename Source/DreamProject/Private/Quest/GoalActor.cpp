// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/GoalActor.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "PaperSprite.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGoalActor::AGoalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComp"));
	RootComponent = DefaultRootComp;

	MiniMapIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MiniMapIcon"));
	MiniMapIcon->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UPaperSprite> Sprite(TEXT("PaperSprite'/Game/Texture/QuestTextures/NpcIcon_Sprite.NpcIcon_Sprite'"));
	MiniMapIcon->SetSprite(Sprite.Object);
	MiniMapIcon->SetRelativeLocation(FVector(0,0,150.f));
	MiniMapIcon->SetRelativeRotation(FRotator(0,0,90));
	MiniMapIcon->SetRelativeScale3D(FVector(2,2,2));
	MiniMapIcon->SetGenerateOverlapEvents(false);
	MiniMapIcon->SetCollisionProfileName(TEXT("NoCollision"));

	MiniMapAreaIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MiniMapAreaIcon"));
	MiniMapAreaIcon->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UPaperSprite> AreaSprite(TEXT("PaperSprite'/Game/Texture/QuestTextures/GoalCircle_Sprite.GoalCircle_Sprite'"));
	MiniMapAreaIcon->SetSprite(AreaSprite.Object);
	MiniMapAreaIcon->SetRelativeLocation(FVector(0,0,150.f));
	MiniMapAreaIcon->SetRelativeRotation(FRotator(0,0,90));
	MiniMapAreaIcon->SetCollisionProfileName(TEXT("NoCollision"));
	MiniMapAreaIcon->SetGenerateOverlapEvents(false);
}


// Called when the game starts or when spawned
void AGoalActor::BeginPlay()
{
	Super::BeginPlay();
	SetOwner(Cast<AActor>(UGameplayStatics::GetPlayerCharacter(this,0)));
	MiniMapIcon->SetOwnerNoSee(true);
	MiniMapAreaIcon->SetOwnerNoSee(true);
}

// Called every frame
void AGoalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoalActor::InitializeMinimapIcon(bool bUseRadius, float Radius, FLinearColor CirecleColor)
{
	MiniMapIcon->SetVisibility(!bUseRadius);
	MiniMapAreaIcon->SetVisibility(bUseRadius);
	if(bUseRadius)
	{
		MiniMapAreaIcon->SetRelativeScale3D(FVector(Radius,Radius,Radius));
		MiniMapAreaIcon->SetSpriteColor(CirecleColor);
	}
}

