// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RBaseCharacter.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "Character/CursorDecal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "DreamProject/DreamProject.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/Skill/BaseSkill.h"
#include "Character/Skill/MissileSkill.h"
#include "Components/SceneCaptureComponent2D.h"
#include "UObject/ConstructorHelpers.h"
#include "UserWidget/MainUserWidget.h"
#include "DreamProject/Public/UserWidget/MainUserWidget.h"
#include "Enemy/NormalEnemy.h"
#include "Engine/DataTable.h"
#include "H:\DreamProject\Source\DreamProject\Public\Character\Info\CharacterInfo.h"
#include "Other/StaticLibrary.h"
#include "Character/Skill/BuffSkill.h"
#include "Components/CapsuleComponent.h"
#include "Components/HorizontalBox.h"
#include "UserWidget/BuffWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/SkillTreeComponent.h"
#include "Interface/InterationInterface.h"
#include "InventorySystem/Inventory.h"
#include "Kismet/GameplayStatics.h"
#include "NPC/BaseNPC.h"
#include "Quest/BaseQuest.h"
#include "Quest/QuestManager.h"
#include "UserWidget/Inventory/InventoryWidget.h"
#include "UserWidget/Quest/QuestJournal.h"
#include "UserWidget/Quest/QuestListEntry.h"
#include "ThreadManage.h"

// Sets default values
ARBaseCharacter::ARBaseCharacter()
	:bIsCasting(false)
{
	//日志输出的方式
	//UE_LOG(LogTemp,Warning,TEXT("Construct"));
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.0f;
	//CameraBoom->SetRelativeRotation(FRotator(-30.0f,0,0));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);

	//显示鼠标
	bUseControllerRotationYaw = false;
	CameraBoom->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	static ConstructorHelpers::FClassFinder<ACursorDecal> DecalAsset(TEXT("/Game/BluePrints/Character/BP_CursorDecal"));
	CursorDecal = DecalAsset.Class;
	
	CurrentLevel = 1;
	NeededExpToNextLevel = FMath::FloorToInt((FMath::Pow((CurrentLevel - 1),3)+60)/5 *((CurrentLevel - 1)*2 + 60) + 60);  //升到第二级所需要的等级

	PortraitComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortraitComponent"));
	PortraitComponent->SetupAttachment(GetMesh(),"head");

	PortraitComponent->SetRelativeLocation(FVector(0,70,0));
	PortraitComponent->SetRelativeRotation(FRotator(0,-90,90));

	Keys.Add(EKeys::One);
	Keys.Add(EKeys::Two);
	Keys.Add(EKeys::Three);
	Keys.Add(EKeys::Four);
	Keys.Add(EKeys::Five);
	Keys.Add(EKeys::Six);
	Keys.Add(EKeys::Seven);
	Keys.Add(EKeys::Eight);
	Keys.Add(EKeys::Nine);
	KeysPerRow = 9;

	SkillTreeComp = CreateDefaultSubobject<USkillTreeComponent>(TEXT("SkillTreeComp"));

	InteractionComp = CreateDefaultSubobject<UCapsuleComponent>("InteractionComp");
	InteractionComp->SetupAttachment(RootComponent);
	InteractionComp->SetRelativeLocation(FVector(120,0,0));
	InteractionComp->SetRelativeRotation(FRotator(90,0,0));
	InteractionComp->SetCapsuleHalfHeight(220);
	InteractionComp->SetCapsuleRadius(100);

	InteractionComp->OnComponentBeginOverlap.AddDynamic(this,&ARBaseCharacter::OnInterActionCompBeginOverlap);
	InteractionComp->OnComponentEndOverlap.AddDynamic(this,&ARBaseCharacter::OnInterActionCompEndOverlap);
}

// Called when the game starts or when spawned
void ARBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//日志输出的方式
	//GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,"BeginPlay");
	PC =Cast<APlayerController>(GetController()) ;
	PC->bShowMouseCursor = true;

	//不管与任何东西发生碰撞都生成
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	//创建仓库
	InventoryRef = GetWorld()->SpawnActor<AInventory>(InventoryClass,Params);

	MainUserWidget = CreateWidget<UMainUserWidget>(GetWorld(),
		LoadClass<UMainUserWidget>(this, TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/WBP_Main.WBP_Main_C'")));
	MainUserWidget->GenerateHotKeys(Keys, KeysPerRow);

	MainUserWidget->InventoryWidget->InventoryRef = InventoryRef;
	MainUserWidget->InventoryWidget->GenerateSlotWidgets();
	//GThread::GetTask().CreateUFunction(MainUserWidget->InventoryWidget,TEXT("GenerateSlotWidgets"));
	
	MainUserWidget->AddToViewport();
	MainUserWidget->QuestManager = QuestManager;
	
	// static ConstructorHelpers::FClassFinder<UMainUserWidget> MainUser_BP(TEXT("/Game/BluePrints/UserWidget/WBP_Main"));
	ReadData();
	IncreaseCurrentExp(0);
	MainUserWidget->SetLevelText(FText::AsNumber(CurrentLevel));

	//生成技能
	//GenerateStartingSkills();

	SkillTreeComp->SetUpTree();
	
	QuestManager = GetWorld()->SpawnActor<AQuestManager>(QuestManagerClass,Params);
	QuestManager->PlayerCharacter = this;
	QuestManager->MainUserWidget = MainUserWidget;

	//MainUserWidget->QuestManager = QuestManager;

	//UE_LOG(LogTemp,Warning,TEXT("%s"),*UStaticLibrary::GetEnumValueAsString<ERegions>("ERegions",ERegions::Grass));
	MainUserWidget->QuestJournal->QInitialize(QuestManager);
	
}

void ARBaseCharacter::MoveForward(float Value)
{
	if(Value != 0 && Controller)
	{
		CancleMissile();
		if(bHasMouseMoveCommand)
		{
			CancleMoveToCursor();
		}
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);

		//获得旋转的的向量
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction,Value);
		QuestManager->OnPlayerMove();
	}
}

void ARBaseCharacter::MoveRight(float Value)
{
	if(Value != 0 && Controller)
	{
		CancleMissile();
		if(bHasMouseMoveCommand)
		{
			CancleMoveToCursor();
		}
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);

		//获得旋转的的向量
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction,Value);
		QuestManager->OnPlayerMove();
	}
}

void ARBaseCharacter::OnSetDestinationPressed()
{
	CancleMissile();
	CancleMoveToCursor();
	bHasMouseMoveCommand = true;
	MoveToCursor();
}

void ARBaseCharacter::SetNewMoveDestination(const FVector DesLocation)
{
	float const Distance = FVector::Dist(DesLocation,GetActorLocation());
	if(Distance > 120.f)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PC,DesLocation);
		QuestManager->OnPlayerMove();
	}
}

void ARBaseCharacter::MoveToCursor()
{
	FHitResult Hit;
	PC->GetHitResultUnderCursor(CursorTraceChannel,false,Hit);
	if(Hit.bBlockingHit)
	{
		//是被高亮选择的物体
		ISelectAbleInterface* SelectAbleInterface = Cast<ISelectAbleInterface>(Hit.GetActor());
		if(SelectAbleInterface)
		{
			//当前点击的Actor是要高亮显示的
			if(SelectActor == Hit.GetActor())
			{
				//两次点击的是同一个物体
				SelectActor = Hit.GetActor();
				if(Cast<ISelectAbleInterface>(SelectActor))
				{
					Cast<ISelectAbleInterface>(SelectActor)->OnSelected(this);
				}
			}
			else//两次点击的不是同一个物体
			{
				//把之前点击的物体取消高亮
				if(Cast<ISelectAbleInterface>(SelectActor))
				{
					Cast<ISelectAbleInterface>(SelectActor)->OnSelectionEnd(this);
				}
				//把当前正在点击的物体设置高亮
				if(Cast<ISelectAbleInterface>(Hit.GetActor()))
				{
					Cast<ISelectAbleInterface>(Hit.GetActor())->OnSelected(this);
					SelectActor = Hit.GetActor();
				}
			}
		}
		else
		{
			if(SelectActor)
			{
				//当点击地面的时候要将之前点击的物体取消高亮
				if(Cast<ISelectAbleInterface>(SelectActor))
				{
					Cast<ISelectAbleInterface>(SelectActor)->OnSelectionEnd(this);
					SelectActor = nullptr;
				}
			}
			FActorSpawnParameters Params;
			Params.Owner = this;
			CurrentCursor = GetWorld()->SpawnActor<ACursorDecal>(CursorDecal,Hit.Location,FRotator::ZeroRotator,Params);
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ARBaseCharacter::CancleMoveToCursor()
{
	if(CurrentCursor)
	{
		bHasMouseMoveCommand = false;
		GetCharacterMovement()->StopMovementImmediately();
		CurrentCursor->Destroy();
		CurrentCursor = nullptr;
	}
}

void ARBaseCharacter::CameraZoomIn()
{
	CameraBoom->TargetArmLength =FMath::Clamp(CameraBoom->TargetArmLength - CameraZoomAlpha,MinCameraZoom_V,MaxCameraZoom_V);
}

void ARBaseCharacter::CameraZoomOut()
{
	CameraBoom->TargetArmLength =FMath::Clamp(CameraBoom->TargetArmLength + CameraZoomAlpha,MinCameraZoom_V,MaxCameraZoom_V);
}


void ARBaseCharacter::ReadData()
{
	UDataTable* CharacterInfo = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(),NULL,
		TEXT("DataTable'/Game/BluePrints/Character/Info/CharacterInfo.CharacterInfo'")));
	if(CharacterInfo)
	{
		FCharacterInfo* Row = CharacterInfo->FindRow<FCharacterInfo>(TEXT("S1"),TEXT("LookUpCharacterInfo"));
		if(Row)
		{
			CharacterName = Row->CharacterName;
			TotalHp = Row->StartHP;
			TotalMp = Row->StartMP;

			CurrentHp = TotalHp;
			CurrentMp = TotalMp;

			ChangeCurrentHp(0);
			ChangeCurrentMp(0);
		}
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("CharacterInfo is not found!"));
	}
}

// void ARBaseCharacter::GenerateStartingSkills()
// {
// 	for(auto Skill : StartingSkills)
// 	{
// 		FActorSpawnParameters Params;
// 		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
// 		Params.Owner = this;
// 		ABaseSkill* TempSkill = GetWorld()->SpawnActor<ABaseSkill>(Skill,Params);
// 		for(auto HotKey : MainUserWidget->GetAllHotkeySlots())
// 		{
// 			if(HotKey && (!HotKey->GetAssignedSpell()))
// 			{
// 				HotKey->AssignSpell(TempSkill);
// 				break;
// 			}
// 		}
// 	}
// }

void ARBaseCharacter::OnAnyKeyPressed(FKey Key)
{
	if(bCanFindKey)
	{
		bCanFindKey = false;
		for(auto HotKeySlot : MainUserWidget->GetAllHotkeySlots())
		{
			// if(PC->IsInputKeyDown(HotKeySlot->GetKey()))
			// {
			// 	if(HotKeySlot->GetAssignedSpell()) //判断技能是否存在
			// 	{
			// 		HotKeySlot->GetAssignedSpell()->OnTryCastSpell();
			// 		break;
			// 	}
			// }
			if(Key == HotKeySlot->GetKey())
			{
				if(HotKeySlot->GetAssignedSpell()) //判断技能是否存在
					{
					HotKeySlot->GetAssignedSpell()->OnTryCastSpell();
					break;
					}
			}
		}
		bCanFindKey = true;
	}
}

void ARBaseCharacter::BeginSpellCast(ABaseSkill* Skill)
{
	bIsCasting = true;
	CurrentSkill = Skill;
	for(auto HotKeySlot : MainUserWidget->GetAllHotkeySlots())
	{
		if(HotKeySlot->GetAssignedSpell())
		{
			if(CurrentSkill != HotKeySlot->GetAssignedSpell())
			{
				HotKeySlot->DisableHotKey();
			}
		}
	}
}

void ARBaseCharacter::EndSpellCast(ABaseSkill* Skill)
{
	bIsCasting = false;
	CurrentSkill = Skill;
	for(auto HotKeySlot : MainUserWidget->GetAllHotkeySlots())
	{
		if(HotKeySlot->GetAssignedSpell())
		{
			if(CurrentSkill != HotKeySlot->GetAssignedSpell())
			{
				HotKeySlot->EnableHotKey();
			}
		}
	}
}

// Called to bind functionality to input
void ARBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&ARBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ARBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp",this,&APawn::AddControllerPitchInput);
	//不是持续的了
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&ACharacter::StopJumping);

	PlayerInputComponent->BindAction("LeftmouseButton",IE_Pressed,this,&ARBaseCharacter::OnSetDestinationPressed);
	PlayerInputComponent->BindAction("ZoomIn",IE_Pressed,this,&ARBaseCharacter::CameraZoomIn);
	PlayerInputComponent->BindAction("ZoomOut",IE_Pressed,this,&ARBaseCharacter::CameraZoomOut);
	PlayerInputComponent->BindAction("AnyKey",IE_Pressed,this,&ARBaseCharacter::OnAnyKeyPressed);

	//显示技能树面板
	PlayerInputComponent->BindAction("ToogleShowSkillTree",IE_Pressed,this,&ARBaseCharacter::ToogleShowSkillTree);

	//NPC
	PlayerInputComponent->BindAction("Interact",IE_Pressed,this,&ARBaseCharacter::InteractToNPC);

	PlayerInputComponent->BindAction("PlaySlideOutAnim",IE_Pressed,this,&ARBaseCharacter::PlaySlideOutAnim);

	PlayerInputComponent->BindAction("TestCompleteQuest",IE_Pressed,this,&ARBaseCharacter::TestCompleteQuest);
	PlayerInputComponent->BindAction("TestFailQuest",IE_Pressed,this,&ARBaseCharacter::TestFailQuest);

	PlayerInputComponent->BindAction("ToogleShowQuest",IE_Pressed,this,&ARBaseCharacter::ToogleShowQuest);
}

void ARBaseCharacter::ChangeCurrentHp(float DeltaHp)
{
	CurrentHp += DeltaHp;
	MainUserWidget->SetHPPProgressBar(CurrentHp/TotalHp);
}

void ARBaseCharacter::ChangeCurrentMp(float DeltaMp)
{
	//设置数值
	CurrentMp += DeltaMp;
	//设置显示
	MainUserWidget->SetMPPProgressBar(CurrentMp/TotalMp);
}

void ARBaseCharacter::IncreaseCurrentExp(float DeltaExp)
{
	CurrentExp += DeltaExp;
	if(CurrentExp >= NeededExpToNextLevel)
	{
		IncreasedLevel();
		CurrentExp -= NeededExpToNextLevel;
	}
	MainUserWidget->SetExpPanel(CurrentExp,NeededExpToNextLevel);
}


float ARBaseCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                  AActor* DamageCauser)
{
	UE_LOG(LogTemp,Warning,TEXT("DamageCauser: %s"),*DamageCauser->GetName());  //DamageCauser造成伤害的人
	ChangeCurrentHp(-1 * Damage);
	return Damage;
}


void ARBaseCharacter::OnReceiveDamage(float BaseDamage, EDamageType Type, TSubclassOf<ABaseElement> Elements,
	int CritChance, AActor* Attacker, ABaseSkill* Spell)
{
	if(UStaticLibrary::bIsEnemy(Attacker) && BaseDamage != 0)
	{
		ChangeCurrentHp(-1 * UStaticLibrary::CaculateFinalDamage(BaseDamage,CritChance,Elements,this->Element));
		UE_LOG(LogTemp,Warning,TEXT("%s"),*FString::SanitizeFloat(UStaticLibrary::CaculateFinalDamage(BaseDamage,CritChance,Elements,this->Element)));
	}
}

void ARBaseCharacter::CancleMissile()
{
	if(MissileSkill)
	{
		//GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->StopMovementImmediately();
		MissileSkill->StopCaculate();
	}
}

UBuffWidget* ARBaseCharacter::AddBuff(ABuffSkill* SkillBuff)
{
	if(Buffs.Contains(SkillBuff))
	{
		return nullptr;
	}
	else
	{
		Buffs.Add(SkillBuff);
		//显示出这个BUff
		BuffWidget = CreateWidget<UBuffWidget>(GetWorld(),
			LoadClass<UBuffWidget>(GetWorld(),TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/WBP_BUff.WBP_Buff_C'")));
		BuffWidget->BuffTexture = SkillBuff->BUffIcon;
		MainUserWidget->BuffBox->AddChildToHorizontalBox(BuffWidget);
		return BuffWidget;
	}
}

void ARBaseCharacter::RemoveBuff(ABuffSkill* SkillBuff)
{
	if(!Buffs.Contains(SkillBuff))
	{
		return;
	}
	else
	{
		Buffs.Remove(SkillBuff);
		BuffWidget->RemoveFromParent();
	}
}

void ARBaseCharacter::IncreasedLevel()
{
	//floor（（(等级 - 1）^3 + 60）/5*（(等级 - 1）*2+60）+60）
	
	CurrentLevel++;
	NeededExpToNextLevel = FMath::FloorToInt((FMath::Pow((CurrentLevel - 1),3)+60)/5 *((CurrentLevel - 1)*2 + 60) + 60);
	MainUserWidget->SetLevelText(FText::AsNumber(CurrentLevel));
	for(UQuestListEntry* QuestListEntry : MainUserWidget->QuestJournal->AllQuestListEntries)
	{
		QuestListEntry->UpdateLevelColor();
	}
	if(MainUserWidget->QuestJournal->CurrentQuestListEntry)
	{
		MainUserWidget->QuestJournal->UpdateSuggestedLevelColor();
	}
	//播放升级的特效
	UGameplayStatics::SpawnEmitterAttached(LevelUpParticle,GetMesh());
}

void ARBaseCharacter::ToogleShowSkillTree()
{
	SkillTreeComp->HandleShowCommand();
}

void ARBaseCharacter::ToogleShowQuest()
{
	if(MainUserWidget->bQuestVisibling)
	{
		MainUserWidget->QuestJournal->SetVisibility(ESlateVisibility::Hidden);
		MainUserWidget->bQuestVisibling = false;
	}
	else
	{
		MainUserWidget->QuestJournal->SetVisibility(ESlateVisibility::Visible);
		MainUserWidget->bQuestVisibling = true;
	}
}

void ARBaseCharacter::OnInterActionCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInterationInterface* InterationInterface =  Cast<IInterationInterface>(OtherActor);
	if(InterationInterface)
	{
		InterationInterface->OnEnterPlayerRadius(this);
	}
}

void ARBaseCharacter::OnInterActionCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInterationInterface* InterationInterface =  Cast<IInterationInterface>(OtherActor);
	if(InterationInterface)
	{
		InterationInterface->OnLeavePlayerRadius(this);
	}
}

void ARBaseCharacter::InteractToNPC()
{
	TArray<AActor*> OverlapActors;
	 InteractionComp->GetOverlappingActors(OverlapActors,TSubclassOf<ABaseNPC>());
	for(AActor* Actor : OverlapActors)
	{
		UE_LOG(LogTemp,Warning,TEXT("Overlap Actor"));
		IInterationInterface* IT = Cast<IInterationInterface>(Actor);
		if(IT)
		{
			IT->OnIteractWith(this);  //与NPC对话
			UE_LOG(LogTemp,Warning,TEXT("接取任务成功"));
			break;
		}
	}
}

void ARBaseCharacter::TestCompleteQuest()
{
	QuestManager->CurrentQuestActors[0]->OnSubGoalCompleted(0,true);
}

void ARBaseCharacter::TestFailQuest()
{
	QuestManager->CurrentQuestActors[0]->OnSubGoalCompleted(1,false);
}

// FString ARBaseCharacter::GetRegionEnumString(ERegions Region)
// {
// 	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE,TEXT("ERegions"),true);
// 	if(!EnumPtr)
// 	{
// 		return FString("InValid");
// 	}
// 	FString TempString = EnumPtr->GetNameByValue((int64)Region).ToString();
// 	TempString.RemoveFromStart("ERegions::");
// 	return TempString;
// }



