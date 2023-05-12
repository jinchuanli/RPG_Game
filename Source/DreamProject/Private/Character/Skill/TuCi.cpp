// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/TuCi.h"

#include "NiagaraFunctionLibrary.h"
#include "VectorTypes.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Enemy/Boss.h"
#include "Enemy/NormalEnemy.h"
#include "Kismet/GameplayStatics.h"

ATuCi::ATuCi()
{
	MyTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("MyTimeLine"));
}

void ATuCi::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATuCi::BeginPlay()
{
	Super::BeginPlay();
	MyTimeLine->SetLooping(false);
	MyTimeLine->SetPlayRate(1.f);
	MyTimeLine->SetPlaybackPosition(0.f,false);
	if(Curver)
	{
		FOnTimelineFloat UpdateTimelineDelegate;
		FOnTimelineEvent OnTimelineFinishedEvent;
		UpdateTimelineDelegate.BindUFunction(this,TEXT("OnTimeLineTick"));
		OnTimelineFinishedEvent.BindUFunction(this,TEXT("OnTimeLineEndEvent"));
		MyTimeLine->AddInterpFloat(Curver,UpdateTimelineDelegate);
		MyTimeLine->SetTimelineFinishedFunc(OnTimelineFinishedEvent);
	}
	
}

void ATuCi::OnTimeLineTick(float Output)
{
	FVector vector = FMath::Lerp(StartLocation,EndLocation,Output);
	PlayerReference->SetActorLocation(vector);
	//UE_LOG(LogTemp, Warning, TEXT("曲线位移"));
}

void ATuCi::OnTimeLineEndEvent()
{
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
}


void ATuCi::OnSkillNotify()
{

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),initiativeEffect,PlayerReference->GetActorLocation(),PlayerReference->GetActorRotation());
	if(StartSound)
		UGameplayStatics::PlaySoundAtLocation(this,StartSound,GetActorLocation());

	TArray<FHitResult> HitResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(GetCurrentStage().Range);

	StartLocation = PlayerReference->GetActorLocation();
	EndLocation = (PlayerReference->GetActorForwardVector())*900.f + PlayerReference->GetActorLocation();

	CapsuleComponent = PlayerReference->GetCapsuleComponent();
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	MyTimeLine->PlayFromStart();

	if(GetWorld()->SweepMultiByChannel(HitResults,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Visibility,
		CollisionShape))
	{
		// UKismetSystemLibrary::DrawDebugSphere(this,PlayerReference->GetActorLocation(),GetCurrentStage().Range,
		// 	12,FLinearColor::Red,3.f,2.f);
		for(auto FHitResult : HitResults)
		{
			if(FHitResult.GetActor())
			{
				ANormalEnemy* Enemy = Cast<ANormalEnemy>(FHitResult.GetActor());
				if(Enemy)
				{
					IDamageAbleInterface* DamageAbleInterface = Cast<IDamageAbleInterface>(Enemy);
					if(DamageAbleInterface)
					{
						DamageAbleInterface->OnReceiveDamage(GetCurrentStage().Damage,GetCurrentStage().DamageType,GetSkillInfo().Element,
							GetCurrentStage().CriticalChance,PlayerReference,this);
					}
				}
				else
				{
					ABoss* Boss = Cast<ABoss>(FHitResult.GetActor());
					if(Boss)
					{
						IDamageAbleInterface* DamageAbleInterface = Cast<IDamageAbleInterface>(Boss);
						if(DamageAbleInterface)
						{
							DamageAbleInterface->OnReceiveDamage(GetCurrentStage().Damage,GetCurrentStage().DamageType,GetSkillInfo().Element,
								GetCurrentStage().CriticalChance,PlayerReference,this);
						}
					}
				}
				
			}
		}
	}
}
