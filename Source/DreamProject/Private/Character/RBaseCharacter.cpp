// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RBaseCharacter.h"
#include <filesystem>
#include <variant>
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Camera/CameraComponent.h"
#include "Character/CursorDecal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "DreamProject/DreamProject.h"
#include "Character/Skill/BaseSkill.h"
#include "Character/Skill/MissileSkill.h"
#include "Components/SceneCaptureComponent2D.h"
#include "UserWidget/MainUserWidget.h"
#include "Enemy/NormalEnemy.h"
#include "Engine/DataTable.h"
#include "Character\Info\CharacterInfo.h"
#include "Other/StaticLibrary.h"
#include "Character/Skill/BuffSkill.h"
#include "Components/CapsuleComponent.h"
#include "Components/HorizontalBox.h"
#include "UserWidget/BuffWidget.h"
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
#include "InventorySystem/ItemStaff.h"
#include "Particles/ParticleSystemComponent.h"
#include "SaveGames/RPGSave.h"
#include "UserWidget/Inventory/ActionMenuWidget.h"
#include "UserWidget/Inventory/CraftingMenuWidget.h"
#include "UserWidget/Inventory/ShopWidget.h"
#include "UserWidget/Inventory/StorageWidget.h"
#include "UserWidget/Inventory/ThrowWidget.h"
#include "AbilitySystemComponent.h"
#include "Character/BaseGameModeBase.h"
#include "Character/MiniCam.h"
#include "Character/RGideon.h"
#include "Components/ArrowComponent.h"
#include "Components/SizeBox.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Enemy/NormalEnemyController.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Gameplay/BaseGameplayAbility.h"
#include "Gameplay/BaseAttributeSet.h"
#include "InventorySystem/Arrow/Arrow.h"
#include "InventorySystem/Arrow/Bow.h"
#include "UObject/ConstructorHelpers.h"
#include "Sword/SwordActor.h"
#include "UserWidget/Revive.h"
#include "UserWidget/Inventory/ItemHotKey.h"
#include "UserWidget/Inventory/ItemHotKeyWidget.h"
#include "UserWidget/SkillTree/MainTreeWidget.h"
#include "UserWidget/SkillTree/SkillTreeEntryWidget.h"

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
	CameraBoom->TargetArmLength = CameraBoomTargetLength;
	
	//CameraBoom->SetRelativeRotation(FRotator(-30.0f,0,0));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);

	OverLoadParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OverLoadParticleComp"));
	OverLoadParticleComp->SetupAttachment(GetMesh());
	OverLoadParticleComp->Deactivate();
	OverLoadParticleComp->bAutoActivate = false;
	
	//显示鼠标
	bUseControllerRotationYaw = false;
	CameraBoom->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	static ConstructorHelpers::FClassFinder<ACursorDecal> DecalAsset(TEXT("/Game/BluePrints/Character/BP_CursorDecal"));
	CursorDecal = DecalAsset.Class;
	
	CurrentLevel = 1;
	NeededExpToNextLevel = FMath::FloorToInt((FMath::Pow((float(CurrentLevel) - 1),3)+60)/5 *((CurrentLevel - 1)*2 + 60) + 60);  //升到第二级所需要的等级

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
	///////////////////////////////////////////////////////////////////

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");

	
	////////////////////////////////////////////////////////////////////////////
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAsset(TEXT("/Game/Animation/UE5/Die_Montage"));
	if(MontageAsset.Succeeded())
	{
		DeathMontage = MontageAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RespawnAsset(TEXT("/Game/Animation/UE5/Respawn_Montage"));
	if(RespawnAsset.Succeeded())
	{
		ResPawnMontage = RespawnAsset.Object;
	}

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

// Called when the game starts or when spawned
void ARBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//日志输出的方式
	//GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,"BeginPlay");
	PC = Cast<ABasePlayerController>(GetController()) ;
	PC->bShowMouseCursor = false;

	MainUserWidget = CreateWidget<UMainUserWidget>(GetWorld(),
	LoadClass<UMainUserWidget>(this, TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/WBP_Main.WBP_Main_C'")));
	MainUserWidget->GenerateHotKeys(Keys, KeysPerRow);
	
	
	DefaultSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	if(!UGameplayStatics::DoesSaveGameExist(SaveSlotName,0))
		ReadData();
	else
	{
		LoadGame();
		MainUserWidget->SetHPPProgressBar(CurrentHp/TotalHp);
		MainUserWidget->SetHPPProgressBar(CurrentMp/TotalMp);
	}

	//不管与任何东西发生碰撞都生成
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	//创建仓库
	InventoryRef = GetWorld()->SpawnActor<AInventory>(InventoryClass,Params);

	MainUserWidget->Inventory = InventoryRef;
	MainUserWidget->InventoryWidget->InventoryRef = InventoryRef;
	MainUserWidget->InventoryWidget->GenerateSlotWidgets();
	MainUserWidget->InventoryWidget->UpdateCoinText(CurrentCoin);
	
	MainUserWidget->AddToViewport();
	MainUserWidget->QuestManager = QuestManager;

	//IncreaseCurrentExp(0);
	NeededExpToNextLevel = FMath::FloorToInt((FMath::Pow((float(CurrentLevel) - 1),3)+60)/5 *((CurrentLevel - 1)*2 + 60) + 60);
	MainUserWidget->SetExpPanel(CurrentExp,NeededExpToNextLevel);
	
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

	MainUserWidget->InventoryWidget->ActionMenu->InventoryRef = InventoryRef;
	MainUserWidget->ThrowWidget->InventoryRef = InventoryRef;
	
	InventoryRef->UpdateWeight();
	MainUserWidget->CraftingMenu->initCraftingMenu(InventoryRef);

	// MainUserWidget->ShopWidget->UpdateCoin();
	// MainUserWidget->ShopWidget->GenerateItemList();
	// MainUserWidget->ShopWidget->SellWidget->InventoryRef = InventoryRef;
	// MainUserWidget->ShopWidget->SellWidget->ShopWidget = MainUserWidget->ShopWidget;

	MainUserWidget->StorageWidget->PlayerInventory = InventoryRef;

	MainUserWidget->GenerateItemHotKeys(ItemKeys);

	MainUserWidget->SkillTreeComponent = this->SkillTreeComp;

	RevivepointLoction = GetActorLocation();
	RevivepointRotation = GetActorRotation();

	AGameModeBase* GM = GetWorld()->GetAuthGameMode();
	GameMode = Cast<ABaseGameModeBase>(GM);
	GameMode->CharacterRef = this;

	//SpawnBow();
}

void ARBaseCharacter::MoveForward(float Value)//MoveForward
{
	if(Value != 0 && Controller)
	{
		CancleMissile();
		// if(bHasMouseMoveCommand)
		// {
		// 	CancleMoveToCursor();
		// }
		
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);
	
		//获得旋转的的向量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction,Value);
		QuestManager->OnPlayerMove();
	}
}

void ARBaseCharacter::MoveRight(float Value) //MoveRight
{
	if(Value != 0 && Controller)
	{
		CancleMissile();
		// if(bHasMouseMoveCommand)
		// {
		// 	CancleMoveToCursor();
		// }
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);
	
		//获得旋转的的向量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction,Value);
		QuestManager->OnPlayerMove();
	}
}

void ARBaseCharacter::OnSetDestinationPressed()
{
	//CancleMissile();
	//CancleMoveToCursor();
	//bHasMouseMoveCommand = true;
	MoveToCursor();
	if(Staff)
	{
		if(Staff->WeaponType == EWeaponType::Sword)
		{
			GetCharacterMovement()->DisableMovement();
			GetCharacterMovement()->StopMovementImmediately();
		}
	}
	//SwordActor->SetCollision();
}

// void ARBaseCharacter::SetNewMoveDestination(const FVector DesLocation)
// {
// 	float const Distance = FVector::Dist(DesLocation,GetActorLocation());
// 	if(Distance > 120.f)
// 	{
// 		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PC,DesLocation);
// 		QuestManager->OnPlayerMove();
// 	}
// }

void ARBaseCharacter::MoveToCursor()
{
	//UE_LOG(LogTemp, Warning, TEXT("点击"));
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
			// FActorSpawnParameters Params;
			// Params.Owner = this;
			// CurrentCursor = GetWorld()->SpawnActor<ACursorDecal>(CursorDecal,Hit.Location,FRotator::ZeroRotator,Params);
			// SetNewMoveDestination(Hit.ImpactPoint);
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
	CameraBoom->TargetArmLength =FMath::Clamp(CameraBoomTargetLength -= CameraZoomAlpha,MinCameraZoom_V,MaxCameraZoom_V);
}

void ARBaseCharacter::CameraZoomOut()
{
	CameraBoom->TargetArmLength =FMath::Clamp(CameraBoomTargetLength += CameraZoomAlpha,MinCameraZoom_V,MaxCameraZoom_V);
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

void ARBaseCharacter::OnAnyKeyPressed(FKey Key) //任意键按下
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
		InventoryRef->HandleItemHotKeyPress(Key);
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
	PlayerInputComponent->BindAction("LeftmouseButton",IE_Released,this,&ARBaseCharacter::ResetMove);
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

	PlayerInputComponent->BindAction("ToogleShowInventory",IE_Pressed,this,&ARBaseCharacter::ToogleShowInventory);
	
	PlayerInputComponent->BindAction("Alt",IE_Pressed,this,&ARBaseCharacter::OnAltPressed);
	PlayerInputComponent->BindAction("Alt",IE_Released,this,&ARBaseCharacter::OnAltReleased);


	
	// PlayerInputComponent->BindAction("Attack", IE_Pressed,this, &ARBaseCharacter::Attacking);
	// PlayerInputComponent->BindAction("Attack", IE_Released,this, &ARBaseCharacter::StopAttacking);
	// PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &ARBaseCharacter::Aiming);
	// PlayerInputComponent->BindAction("Aiming", IE_Released, this, &ARBaseCharacter::StopAiming);
}

void ARBaseCharacter::ChangeCurrentHp(float DeltaHp)
{
	CurrentHp  = FMath::Clamp(CurrentHp + DeltaHp,0.f,TotalHp);
	MainUserWidget->SetHPPProgressBar(CurrentHp/TotalHp);
	UE_LOG(LogTemp, Warning, TEXT("改变当前的血量 %f"),CurrentHp);

	////////////////////////////////
	if(CurrentHp <= 0.2  && bDead ==false)
		DeadCharacter();
}

void ARBaseCharacter::ChangeCurrentMp(float DeltaMp)
{
	//设置数值
	CurrentMp = FMath::Clamp(CurrentMp + DeltaMp,0.f,TotalMp);
	UE_LOG(LogTemp, Warning, TEXT("%f"),CurrentMp);
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
	NeededExpToNextLevel = FMath::FloorToInt((FMath::Pow((float(CurrentLevel) - 1),3)+60)/5 *((CurrentLevel - 1)*2 + 60) + 60);
	MainUserWidget->SetLevelText(FText::AsNumber(CurrentLevel));
	for(UQuestListEntry* QuestListEntry : MainUserWidget->QuestJournal->AllQuestListEntries)
	{
		QuestListEntry->UpdateLevelColor();
	}
	if(MainUserWidget->QuestJournal->CurrentQuestListEntry)
	{
		MainUserWidget->QuestJournal->UpdateSuggestedLevelColor();
	}
	SkillTreeComp->SkillPoints++;
	MainUserWidget->SkillTree->UpdateSp();
	SkillTreeComp->UpdateAllEntries();
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
		FInputModeGameOnly InputMode;
		PC->bEnableClickEvents = false;
		PC->bShowMouseCursor = false;
		PC->SetInputMode(InputMode);
		MainUserWidget->bQuestVisibling = false;
	}
	else
	{
		MainUserWidget->QuestJournal->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->bEnableClickEvents = true;
		PC->bShowMouseCursor = true;
		MainUserWidget->bQuestVisibling = true;
	}
}

void ARBaseCharacter::OnInterActionCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInterationInterface* InterationInterface =  Cast<IInterationInterface>(OtherActor);
	AItemStaff* TempSatff = Cast<AItemStaff>(OtherActor);
	if(InterationInterface)
	{
		if(TempSatff)
		{
			if(TempSatff != Staff)
			{
				InterationInterface->OnEnterPlayerRadius(this);
			}
		}
		else
		{
			InterationInterface->OnEnterPlayerRadius(this);
		}
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
		//UE_LOG(LogTemp,Warning,TEXT("Overlap Actor"));
		IInterationInterface* IT = Cast<IInterationInterface>(Actor);
		if(IT)
		{
			IT->OnIteractWith(this);  //与NPC对话
			//UE_LOG(LogTemp,Warning,TEXT("接取任务成功"));
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

void ARBaseCharacter::ToogleShowInventory()
{
	if(MainUserWidget->bInventoryVisibling)
	{
		MainUserWidget->InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameOnly InputMode;
		PC->bEnableClickEvents = false;
		PC->bShowMouseCursor = false;
		PC->SetInputMode(InputMode);
		MainUserWidget->bInventoryVisibling = false;
	}
	else
	{
		MainUserWidget->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->bEnableClickEvents = true;
		PC->bShowMouseCursor = true;
		MainUserWidget->bInventoryVisibling = true;
	}
}

void ARBaseCharacter::OnAltPressed()
{
	PC->bShowMouseCursor = true;
	//PC->ShowMouseSetupInputComponent();
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputMode);
	PC->bEnableClickEvents = true;
	bAltDown = true;
}

void ARBaseCharacter::OnAltReleased()
{
	PC->bShowMouseCursor = false;
	//PC->StopShow();
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->bEnableClickEvents = false;
	bAltDown = false;
}

bool ARBaseCharacter::EquipItem(AItemStaff* ItemStaff)
{
	if(!Staff)
	{
		if(InventoryRef->RemoveItemAtIndex(ItemStaff->Index,1))
		{
			Staff = ItemStaff;
			Staff->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,"hand_r_sword");
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		//1.UnEquip,2,Equip
		if(UnEquipItem())
		{
			return EquipItem(ItemStaff);
		}
		else
		{
			return false;
		}
	}
}

bool ARBaseCharacter::EquipItem_Weapon(AItemStaff* ItemStaff)
{
	if(!Staff)
	{
		if(ItemStaff->ItemInfo.Category == EItemCategories::Equipment)
		{
			Staff = ItemStaff;
			Staff->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,"hand_r_sword");
			for(UItemHotKey* HotKey : this->MainUserWidget->ItemHotKeyWidgets)
			{
				if(!HotKey->ItemHotKeySlot->bEmpty && HotKey->ItemHotKeySlot->InventoryIndex == ItemStaff->Index)
				{
					HotKey->ItemHotKeySlot->UseState->SetRenderScale(FVector2d(1.2f,1.2f));
				}
			}
			return true;
		}
		else if(ItemStaff->ItemInfo.Category == EItemCategories::Bow)
		{
			Staff = ItemStaff;
			Bow = Cast<ABow>(Staff);
			if(Bow)
			{
				auto BowMesh = Bow->GetBowMesh();
				FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget,false);
				BowMesh->AttachToComponent(this->GetMesh(),Rules,"SocketForBow");
				NowVelocity = Bow->GetBaseVelocity();
				for(UItemHotKey* HotKey : this->MainUserWidget->ItemHotKeyWidgets)
				{
					if(!HotKey->ItemHotKeySlot->bEmpty && HotKey->ItemHotKeySlot->InventoryIndex == ItemStaff->Index)
					{
						HotKey->ItemHotKeySlot->UseState->SetRenderScale(FVector2d(1.2f,1.2f));
					}
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(UnEquipItem_Weapon())
		{
			return EquipItem_Weapon(ItemStaff);
		}
		else
		{
			return false;
		}
	}
}

bool ARBaseCharacter::UnEquipItem_Weapon()
{
	if(Staff)
	{
		for(UItemHotKey* HotKey : this->MainUserWidget->ItemHotKeyWidgets)
		{
			if(!HotKey->ItemHotKeySlot->bEmpty && HotKey->ItemHotKeySlot->InventoryIndex == Staff->Index)
			{
				HotKey->ItemHotKeySlot->UseState->SetRenderScale(FVector2d(1.0f,1.0f));
				UE_LOG(LogTemp, Warning, TEXT("HotKey->ItemHotKeySlot->UseState->SetRenderScale(FVector2d(1.0f,1.0f));"));
			}
		}
		Staff->Destroy();
		Staff = nullptr;
		
		return true;
	}
	else
	{
		return false;
	}
}


bool ARBaseCharacter::UnEquipItem()
{
	if(Staff)
	{//卸下并且放回背包
		if(InventoryRef->AddItem(Staff->GetClass(),1) == 0)
		{
			Staff->Destroy();
			Staff = nullptr;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void ARBaseCharacter::OnOverLoaded()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed / 2;
	OverLoadParticleComp->Activate();
}

void ARBaseCharacter::OnOverLoadEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	OverLoadParticleComp->Deactivate();
}

void ARBaseCharacter::IncreaseCoin(int Amount)
{
	if(Amount > 0)
	{
		CurrentCoin += Amount;
		MainUserWidget->InventoryWidget->UpdateCoinText(CurrentCoin);
		if(MainUserWidget->ShopWidget)
			MainUserWidget->ShopWidget->UpdateCoin();
	}
}

void ARBaseCharacter::DecreaseCoin(int Amount)
{
	CurrentCoin = FMath::Clamp(CurrentCoin - Amount,0,CurrentCoin);
	MainUserWidget->InventoryWidget->UpdateCoinText(CurrentCoin);
	if(MainUserWidget->ShopWidget)
		MainUserWidget->ShopWidget->UpdateCoin();
}

void ARBaseCharacter::SaveGame()
{
	//创建
	if(!RPDSaveInstance)
		RPDSaveInstance = Cast<URPGSave>(UGameplayStatics::CreateSaveGameObject(URPGSave::StaticClass()));

	//保存信息
	RPDSaveInstance->SavedName = CharacterName;
	RPDSaveInstance->SavedHP = CurrentHp;
	RPDSaveInstance->SaveMP = CurrentMp;
	RPDSaveInstance->SaveEXP = CurrentExp;
	RPDSaveInstance->SavedLevel = CurrentLevel;
	RPDSaveInstance->SavedCoin = CurrentCoin;

	//存贮
	UGameplayStatics::SaveGameToSlot(RPDSaveInstance,SaveSlotName,0);
}

void ARBaseCharacter::LoadGame()
{
	//加载
	if(!RPDSaveInstance)
		RPDSaveInstance = Cast<URPGSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName,0));
	//赋值
	CharacterName = RPDSaveInstance->SavedName;
	CurrentHp = RPDSaveInstance->SavedHP;
	CurrentMp = RPDSaveInstance->SaveMP;
	CurrentExp = RPDSaveInstance->SaveEXP;
	CurrentLevel = RPDSaveInstance->SavedLevel;
	CurrentCoin = RPDSaveInstance->SavedCoin;
}

UAbilitySystemComponent* ARBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARBaseCharacter::AquireAbility(TSubclassOf<UBaseGameplayAbility> Ability)
{
	if(AbilitySystemComponent && Ability)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
	}
}

void ARBaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AddChargingVelocity();
	DestoryLine();
	DrawLine();
}

void ARBaseCharacter::ResetMove()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ARBaseCharacter::DeadCharacter()
{
	bDead = true;
	//GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputModeUIOnly);
	MainUserWidget->SetVisibility(ESlateVisibility::Hidden);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
		// if(bDead)
		// 	GetWorldTimerManager().SetTimer(TimerHandle_Simlate,this,&ARBaseCharacter::SetSimlate,(DeathMontage->GetPlayLength()) * 0.5f,false);
		GetWorldTimerManager().SetTimer(TimerHandle_Dead,this,&ARBaseCharacter::SetDeadGameMode,DeathMontage->GetPlayLength(),false);
		UE_LOG(LogTemp, Warning, TEXT("DeathMontage exist"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DeathMontage not exist"));
	}
}

void ARBaseCharacter::SetDeadGameMode()
{
	PC->bShowMouseCursor = true;
	GetWorldTimerManager().ClearTimer(TimerHandle_Dead);
	//GetController()->UnPossess();
	ShowReviveUI();
}

void ARBaseCharacter::ShowReviveUI()
{
	Revive = CreateWidget<URevive>(GetWorld(),
		LoadClass<URevive>(this,TEXT("WidgetBlueprint'/Game/BluePrints/UserWidget/WBP_Revive.WBP_Revive_C'")));
	Revive->CharacterRef = this;
	Revive->AddToViewport();
}

void ARBaseCharacter::SetSimlate()
{
	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
	if(SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetSimulatePhysics(bDead);
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		UE_LOG(LogTemp, Warning, TEXT("RGideon have"));
	}
	GetWorldTimerManager().ClearTimer(TimerHandle_Simlate);
}

void ARBaseCharacter::Respawn()
{
	if(bDead)
	{
		if(ResPawnMontage)
		{
			bDead = false;
			SetActorLocation(RevivepointLoction);
			PlayAnimMontage(ResPawnMontage);
			Revive->SetVisibility(ESlateVisibility::Collapsed);
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			MainUserWidget->SetVisibility(ESlateVisibility::Visible);
			CurrentHp = GetTotalHp();
			MainUserWidget->SetHPPProgressBar(CurrentHp/TotalHp);
			CurrentMp = GetTotalMp();
			MainUserWidget->SetMPPProgressBar(CurrentMp/TotalMp);
		}
	}
}

void ARBaseCharacter::SpawnBow()
{
	if(GetWorld())
	{
		//创建了实例
		Bow = GetWorld()->SpawnActor<ABow>(BowClass);
		auto BowMesh = Bow->GetBowMesh();

		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget,false);
		BowMesh->AttachToComponent(this->GetMesh(),Rules,"SocketForBow");

		NowVelocity = Bow->GetBaseVelocity();
	}
}

void ARBaseCharacter::Attacking()
{
	if(bAnimingMode)
	{
		bCharging = true;
	}
}

void ARBaseCharacter::StopAttacking()
{
	if(bCanAttck && bArrowSpawned)
	{
		LaunchArrow();
	}
	bCharging = false;
	//重置基本速度
	NowVelocity = Bow->GetBaseVelocity();
}

void ARBaseCharacter::LaunchArrow()
{
	Arrow->EnableDamege = true;
	Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	auto Sphere = Arrow->GetSphere();
	auto Projectile = Arrow->GetProjectileMovement();

	Sphere->IgnoreActorWhenMoving(this,true);

	Projectile->ProjectileGravityScale = 1;
	Projectile->Velocity = NowFvectorVelocity;

	bArrowSpawned = false;
	bCanAttck = false;
	bDrawLine = false;
	bBowStringAttackToHand = false;

	auto BowTime = Bow->GetReloadTime();
	auto Time = DrawArrowAnim->GetTimeAtFrame(60);
	float Rate = Time/BowTime;

	//播放动画的速率
	DrawArrowAnim->RateScale = Rate;
	this->PlayAnimMontage(DrawArrowAnim);
}

void ARBaseCharacter::AddChargingVelocity()
{
	if(bCharging)
	{
		NowVelocity += Bow->GetChargingStep();

		NowVelocity = FMath::Clamp(NowVelocity,0,Bow->GetMaxVelocity());
	}
}

void ARBaseCharacter::Aiming()
{
	bAnimingMode = true;

	auto BowTime = Bow->GetReloadTime();
	auto Time = DrawArrowAnim->GetTimeAtFrame(60);
	float Rate = Time/BowTime;

	DrawArrowAnim->RateScale = Rate;
	this->PlayAnimMontage(DrawArrowAnim);

	bUseControllerRotationYaw = true;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->bOrientRotationToMovement = false;
}

void ARBaseCharacter::StopAiming()
{
	this->StopAnimMontage();

	bAnimingMode = false;
	bDrawLine = false;
	bBowStringAttackToHand = false;

	if(Arrow->IsAttachedTo(this))
	{
		Arrow->Destroy();
	}

	bUseControllerRotationYaw = false;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->bOrientRotationToMovement = true;
}

void ARBaseCharacter::DrawLine()
{
	if(bDrawLine)
	{
		FPredictProjectilePathParams Params;
		FPredictProjectilePathResult Result;

		//得到方向（力）
		NowFvectorVelocity = this->GetController()->GetControlRotation().Vector().GetSafeNormal();
		NowFvectorVelocity *= NowVelocity;  //得力
		//UE_LOG(LogTemp, Warning, TEXT("方向 %s"),*NowFvectorVelocity.ToString());
		
		Params.ActorsToIgnore.Add(this);
		Params.ActorsToIgnore.Add(Arrow);

		Params.StartLocation = Arrow->GetActorLocation();
		Params.MaxSimTime = 3.0f;
		Params.bTraceWithCollision = true;
		Params.LaunchVelocity = NowFvectorVelocity;
		Params.ProjectileRadius = 1.f;
		Params.SimFrequency = 10;
		Params.DrawDebugType = EDrawDebugTrace::None;

		UGameplayStatics::PredictProjectilePath(Arrow,Params,Result);

		Spline->ClearSplinePoints();

		for(int32 i = 0;i < Result.PathData.Num();i++)
		{
			Spline->AddSplinePoint(Result.PathData[i].Location,ESplineCoordinateSpace::Local,true);
		}

		FVector PrePointLocation;
		FVector NextPointLocation;
		FVector PrePointTangent;
		FVector NextPointTangent;

		for(size_t i =0;i < Spline->GetNumberOfSplinePoints()-1;i++)
		{
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this,USplineMeshComponent::StaticClass());
			SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;

			SplineMesh->SetStaticMesh(SplineStaticMesh);
			SplineMesh->SetForwardAxis(ESplineMeshAxis::X,false);

			PrePointLocation = Spline->GetLocationAtSplinePoint(i,ESplineCoordinateSpace::Local);
			PrePointTangent = Spline->GetTangentAtSplinePoint(i,ESplineCoordinateSpace::Local);
			NextPointLocation = Spline->GetLocationAtSplinePoint(i + 1,ESplineCoordinateSpace::Local);
			NextPointTangent = Spline->GetTangentAtSplinePoint(i + 1,ESplineCoordinateSpace::Local);

			SplineMesh->SetStartAndEnd(PrePointLocation,PrePointTangent,NextPointLocation,NextPointTangent);

			SplineMeshs.Add(SplineMesh);
		}
		RegisterAllComponents();
	}
}

void ARBaseCharacter::DestoryLine()
{
	for(size_t i = 0; i < SplineMeshs.Num(); i++)
	{
		SplineMeshs[i]->DestroyComponent();
	}
	SplineMeshs.Empty();
}

void ARBaseCharacter::WhenDrawArrowNotify()
{
	if(GetWorld())
	{
		Arrow = GetWorld()->SpawnActor<AArrow>(ArrowClass);
		auto ArrowSphere = Arrow->GetSphere();  //得到碰撞体

		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget,false);

		ArrowSphere->AttachToComponent(this->GetMesh(),Rules,"SocketForArrow");

		bArrowSpawned = true;
	}
}

void ARBaseCharacter::WhenDrawArrowEndNotify()
{
	bDrawLine = true;
	bCanAttck = true;
}

void ARBaseCharacter::WhenDrawBowStringNotify()
{
	bBowStringAttackToHand = true;
}
