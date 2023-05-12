// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/Evade.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"

AEvade::AEvade()
{
	MyTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("MyTimeLine"));
}

void AEvade::BeginPlay()
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

void AEvade::OnTimeLineTick(float Output)
{
	FVector Vector = FMath::Lerp(StartLocation,TargetLocation,Output);
	PlayerReference->SetActorLocation(Vector);
}

void AEvade::OnTimeLineEndEvent()
{
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
}

void AEvade::OnSkillNotify()
{
	StartLocation = PlayerReference->GetActorLocation();
	TargetLocation = (PlayerReference->GetActorForwardVector()*GetCurrentStage().Range) + StartLocation;
	CapsuleComponent = PlayerReference->GetCapsuleComponent();
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	MyTimeLine->PlayFromStart();
	if(InitiativeEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),InitiativeEffect,StartLocation,PlayerReference->GetActorRotation());
	if(StartSound)
		UGameplayStatics::PlaySoundAtLocation(this,StartSound,StartLocation);
}
