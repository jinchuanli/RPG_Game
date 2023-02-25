// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/BaseSkill.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UserWidget/SkillHotKey.h"
#include "TimerManager.h"
#include "UserWidget/MainUserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseSkill::ABaseSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseSkill::BeginPlay()
{
	Super::BeginPlay();
	PlayerReference = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	AnimInstance = PlayerReference->GetMesh()->GetAnimInstance();
}

void ABaseSkill::InitializeSpellCast()
{
	if(PlayerReference->GetCurrentMp() >= GetCurrentStage().ManaCost)
	{
		bCurrentCasting = true;
		PlayerReference->BeginSpellCast(this);
		PlayerReference->ChangeCurrentMp(GetCurrentStage().ManaCost*-1);
		OnSpellCast();
	}
	else
	{
		//UE_LOG(LogTemp,Warning,TEXT("Not Enough Mana!"));
		//GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,"Not Enough Mana!");
		PlayerReference->MainUserWidget->PlayPopUpAnimation();
	}
}

void ABaseSkill::OnSpellCast()
{
	//UE_LOG(LogTemp,Warning,TEXT("Casting skill: %s"),*SkillInfo.Name.ToString());
	//
	// FString a = *SkillInfo.Name.ToString();
	// FString b = "Casting skill ";
	// GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,b+a);

	if(SkillAnimMontage)
	{
		PlaySkillAnimation(SkillAnimMontage);
	}
	OnCastCompleted();
}

void ABaseSkill::OnCastCompleted()
{
	PlayerReference->EndSpellCast(this);
	bCurrentCasting = false;
	bOnCoolDown = true;
	if(HotKey)
	{
		HotKey->SkillButton->SetIsEnabled(false);
		HotKey->SkillIcon->SetColorAndOpacity(FLinearColor(0.33,.033,0.33,1));
		HotKey->CooldownMat->SetScalarParameterValue("Percent",1);
		HotKey->CoolDownImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HotKey->CoolDownText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CurrentCD = GetCurrentStage().CooldownTime;
		GetWorldTimerManager().SetTimer(TimerHandle_CoolDown,this,&ABaseSkill::OnCoolDown,0.01f,true);
	}
}

void ABaseSkill::OnCoolDown()
{
	if(CurrentCD <= 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_CoolDown);
		OnCoolDownExpired();
	}
	else
	{
		CurrentCD -= 0.01f;
		HotKey->CooldownMat->SetScalarParameterValue("Percent",CurrentCD/GetCurrentStage().CooldownTime);
		if(CurrentCD >= 1.f)
			HotKey->CoolDownText->SetText(FText::AsNumber(FMath::RoundHalfToZero(CurrentCD)));
		else
		{
			HotKey->CoolDownText->SetText(FText::AsNumber(FMath::RoundHalfToZero(10.f * CurrentCD) / 10.f));
		}
	}
	
}

void ABaseSkill::OnCoolDownExpired()
{
	bOnCoolDown = false;
	if(HotKey)
	{
		if(!HotKey->bDeactive)
		{
			HotKey->SkillButton->SetIsEnabled(true);
			HotKey->SkillIcon->SetColorAndOpacity(FLinearColor::White);
		}
		HotKey->CooldownMat->SetScalarParameterValue("Percent",0);
		HotKey->CoolDownImage->SetVisibility(ESlateVisibility::Hidden);
		HotKey->CoolDownText->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Called every frame
void ABaseSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseSkill::OnTryCastSpell()
{
	if(!bOnCoolDown && !bCurrentCasting && !PlayerReference->GetbIsCasting())
	{
		InitializeSpellCast();
	}
}

//播放技能动画
void ABaseSkill::PlaySkillAnimation(UAnimMontage* SkillAnimtion)
{
	//禁用移动
	PlayerReference->GetCharacterMovement()->DisableMovement();
	//停止移动
	PlayerReference->GetCharacterMovement()->StopMovementImmediately();

	AnimInstance->Montage_Play(SkillAnimtion);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetMove,this,&ABaseSkill::RestMovement,SkillAnimtion->SequenceLength,false);
}

void ABaseSkill::RestMovement()
{
	PlayerReference->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ABaseSkill::OnSkillNotify()
{
	GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,"OnSkillNotify");
}

