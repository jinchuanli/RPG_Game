// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NormalEnemy.h"

#include "Character/RBaseCharacter.h"
#include "Character/Skill/BaseElement.h"
#include "Enemy/NormalEnemyController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Components/SphereComponent.h"
#include "DreamProject/DreamProject.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "UserWidget/EnemyInfoWidget.h"
#include "Math/Color.h"
#include "Components/TextBlock.h"
#include "Other/StaticLibrary.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Components/Border.h"
#include "UserWidget/MainUserWidget.h"
#include "Components/ArrowComponent.h"
#include "Quest/QuestManager.h"

#define LOCTEXT_NAMESPACE "EnemyNameSpace"


// Sets default values
ANormalEnemy::ANormalEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ANormalEnemyController::StaticClass();

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	if(AIPerceptionComp)
	{
		SightConfig->SightRadius = 800;
		SightConfig->LoseSightRadius = 2000;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	}

	//绑定
	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->SetDominantSense(UAISense_Sight::StaticClass());

	//注册视觉组件
	AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this,&ANormalEnemy::OnSightPerceptionUpdated);

	EnemyWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyWidgetComp"));
	//在文件中找到这个类
	static ConstructorHelpers::FClassFinder<UEnemyInfoWidget> EIW(TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/Enemy/WBP_EnemyInfo.WBP_EnemyInfo_C'"));
	EnemyWidgetComp->SetWidgetClass(EIW.Class);

	EnemyWidgetComp->SetDrawSize(FVector2D(200.0f,60.0f));
	EnemyWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyWidgetComp->SetTwoSided(true);
	EnemyWidgetComp->SetVisibility(false);
	
	EnemyWidgetComp->SetupAttachment(RootComponent);
	EnemyWidgetComp->SetRelativeLocation(FVector(0,0,100));

	ShowUICollision = CreateDefaultSubobject<USphereComponent>(TEXT("ShowUICollision"));
	ShowUICollision->SetupAttachment(RootComponent);
	//动态绑定
	ShowUICollision->OnComponentBeginOverlap.AddDynamic(this,&ANormalEnemy::OnBeginOverlap);
	ShowUICollision->OnComponentEndOverlap.AddDynamic(this,&ANormalEnemy::OnEndOverlap);

	HitArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("HitArrow"));
	HitArrow->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();

	//获得组件
	EnemyInfoWidget = Cast<UEnemyInfoWidget>(EnemyWidgetComp->GetUserWidgetObject());

	MyController = Cast<ANormalEnemyController>(GetController());
	MyController->Patrol();

	StartLocation = GetActorLocation();
	EnemyWidgetComp->SetVisibility(bInShowRange);
	InitWidgetText();
	CurrentHealth = TotalHealth;
	UpdateHealthBar();
}

// Called every frame
void ANormalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANormalEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANormalEnemy::OnSightPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if(bAggressive && !MyController->bWasAggroed)
	{
		for(auto Actor : UpdatedActors)
			MyController->OnAggroedPulled(Cast<ARBaseCharacter>(Actor));
	}
}

void ANormalEnemy::NotifyHit_M()
{
	AttackRay();
}

void ANormalEnemy::AttackRay()
{
	int Range = 250;
	FVector MyStartLocation = GetActorLocation();
	FVector EndLocation = MyStartLocation+GetCapsuleComponent()->GetForwardVector()*Range;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if(GetWorld()->LineTraceSingleByChannel(HitResult,StartLocation,EndLocation,EnemyHitChannel,QueryParams))
	{
		ARBaseCharacter *Character = Cast<ARBaseCharacter>(HitResult.GetActor());
		if(Character)
		{
			IDamageAbleInterface* DamageAbleInterface = Cast<IDamageAbleInterface>(Character);
			if(DamageAbleInterface)
			{
				DamageAbleInterface->OnReceiveDamage(BaseDamage,DamageType,Element,CritChance,this,nullptr);
			}
		}
	}
}

void ANormalEnemy::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp,Warning,TEXT("执行显示血条"));
	if(!bDead)
	{
		if(Cast<ARBaseCharacter>(OtherActor) && !EnemyWidgetComp->IsVisible())
		{
			bInShowRange = true;
			EnemyWidgetComp->SetVisibility(true);
			//UE_LOG(LogTemp,Warning,TEXT("显示血条"));
		}
		// else
		// {
		// 	UE_LOG(LogTemp,Warning,TEXT("显示血条出错"));
		// }
	}
}

void ANormalEnemy::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!bDead)
	{
		if(Cast<ARBaseCharacter>(OtherActor) && EnemyWidgetComp->IsVisible())
		{
			bInShowRange = false;
			EnemyWidgetComp->SetVisibility(false);
		}
	}
}

void ANormalEnemy::InitWidgetText()
{
	EnemyInfoWidget->LevelName->SetText(FText::Format(LOCTEXT("EnemyNameSpace","[Lv{0}.{1}]"),FText::AsNumber(Level),Name));
	FLinearColor CurrentColor;
	if(bAggressive)
	{
		CurrentColor = FLinearColor::Red;
	}
	else
	{
		CurrentColor = FLinearColor::White;
	}
	EnemyInfoWidget->SetColorAndOpacity(CurrentColor);
}

void ANormalEnemy::UpdateHealthBar()
{
	EnemyInfoWidget->HealthBar->SetPercent(CurrentHealth / TotalHealth);
	if(bSelected)
	{
		SelectingCharacter->MainUserWidget->EnemyHpBar->SetPercent(CurrentHealth/TotalHealth);
		SelectingCharacter->MainUserWidget->EnemyHpText->SetText(FText::Format(LOCTEXT("EnemyNameSpace","{0}/{1}"),FText::AsNumber(FMath::RoundHalfToZero(CurrentHealth)),
			FText::AsNumber(TotalHealth)));
	}
}

void ANormalEnemy::OnReceiveDamage(float FBaseDamage, EDamageType Type, TSubclassOf<ABaseElement> FElement,
	int FCritChance, AActor* Attacker, ABaseSkill* Spell)
{
	if(!bDead && !UStaticLibrary::bIsEnemy(Attacker))
	{
		CurrentHealth = FMath::Clamp((CurrentHealth - UStaticLibrary::CaculateFinalDamage(FBaseDamage,FCritChance,FElement,this->Element)),0.f,TotalHealth);
		UpdateHealthBar();
		if(CurrentHealth <= 0.5)
		{
			OnDeath(Attacker);
			
		}
		else
		{
			//进入狂暴的状态
			MyController->OnAggroedPulled(Attacker);
		}
	}
}

void ANormalEnemy::OnDeath(AActor* Killer)
{
	MyController->AnimInstance->Montage_Stop(0.f);
	bDead = true;
	//一般需要一个物体停止移动都需要这两句话
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	EnemyWidgetComp->SetVisibility(false);
	
	//判断主角获得经验
	ARBaseCharacter* TempCharacter = Cast<ARBaseCharacter>(Killer);
	if(TempCharacter)
	{
		TempCharacter->IncreaseCurrentExp(ExpForKill);
		if(TempCharacter->SelectedEnemy == this)
		{
			TempCharacter->SelectedEnemy = nullptr;
			TempCharacter->MainUserWidget->EnemyBorder->SetVisibility(ESlateVisibility::Hidden);
			
		}
		TempCharacter->QuestManager->OnEnemyKilled(this->GetClass());
	}

	
	//禁用碰撞
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().SetTimer(TimerHandle_DelayDeath,this,&ANormalEnemy::OnDelayDeath,2.0f,false);
}

void ANormalEnemy::Respawn()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bDead = false;
	SetActorLocation(StartLocation);
	CurrentHealth = TotalHealth;
	UpdateHealthBar();
	InitWidgetText();
	MyController->bIsPatrolling = true;
	MyController->bWasAggroed = false;
	MyController->TargetActor = nullptr;
	MyController->CurrentAttackIndex = 0;
	MyController->bIsRunningBack = false;
	GetCharacterMovement()->MaxWalkSpeed = MyController->PatrolWalkSpeed;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	EnemyWidgetComp->SetVisibility(bInShowRange);
	SetActorHiddenInGame(true);
	MyController->Patrol();
}

void ANormalEnemy::OnSelected(ARBaseCharacter* Character)
{
	if(!bDead && !bSelected)
	{
		SelectingCharacter = Character;
		bSelected = true;
		GetMesh()->SetRenderCustomDepth(true); // 产生高亮
		Character->MainUserWidget->EnemyNameLevel->SetText(FText::Format(LOCTEXT("EnemyNameSpace","{0}(Lv.{1})"),
			Name,FText::AsNumber(Level)));
		UpdateHealthBar();
		Character->MainUserWidget->EnemyBorder->SetVisibility(ESlateVisibility::Visible);
		Character->SelectedEnemy = this;
	}
}

void ANormalEnemy::OnSelectionEnd(ARBaseCharacter* Character)
{
	if(!bDead && bSelected)
	{
		GetMesh()->SetRenderCustomDepth(false);
		bSelected = false;
		Character->MainUserWidget->EnemyBorder->SetVisibility(ESlateVisibility::Hidden);
		SelectingCharacter = nullptr;
		Character->SelectedEnemy = nullptr;
	}
}

void ANormalEnemy::OnDelayDeath()
{
	//设置隐藏
	SetActorHiddenInGame(true);
	//选择间隔时间重生
	if(bDoesRespawn)
		GetWorldTimerManager().SetTimer(TimerHandle_Respawn,this,&ANormalEnemy::Respawn,RespawnTime,false);
	else
		Destroy();
}

#undef LOCTEXT_NAMESPACE

//1，想使用字符格式化的功能必须定义命名空间

