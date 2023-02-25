// Fill out your copyright notice in the Description page of Project Settings.


#include "Other/DamageArea.h"

#include "Character/RBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADamageArea::ADamageArea()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(RootComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ADamageArea::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this,&ADamageArea::OnEndOverlap);
}

void ADamageArea::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<ARBaseCharacter>(OtherActor))
	{
		Player = Cast<ARBaseCharacter>(OtherActor);
		GetWorldTimerManager().SetTimer(TimerHandle_Damage,this,&ADamageArea::ApplayAreaDamage,1.f,true);
	}
}

void ADamageArea::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Cast<ARBaseCharacter>(OtherActor))
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Damage);
	}
}

void ADamageArea::ApplayAreaDamage()
{
	//伤害的发起
	UGameplayStatics::ApplyDamage(Player,5.f,nullptr,this,nullptr);
}



