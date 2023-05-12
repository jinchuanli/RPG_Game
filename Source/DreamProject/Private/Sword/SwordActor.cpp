// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword/SwordActor.h"

#include "Character/RBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/NormalEnemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASwordActor::ASwordActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	RootComponent = Weapon;
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called when the game starts or when spawned
void ASwordActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASwordActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwordActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANormalEnemy* TempEnemy = Cast<ANormalEnemy>(OtherActor);
	if(TempEnemy && OtherComp == TempEnemy->GetCapsuleComponent())
	{
		//传递伤害
		IDamageAbleInterface* DamageAbleInterface = Cast<IDamageAbleInterface>(TempEnemy);
		if(DamageAbleInterface)
		{
			DamageAbleInterface->OnReceiveDamage(
				10.f,
				EDamageType::Physical,
				nullptr,
				10,
				this,
				nullptr);
		}
	}
}

// void ASwordActor::SetCollision()
// {
// 	BoxCollisonComp->SetGenerateOverlapEvents(true);
// 	if(BoxCollisonComp->GetGenerateOverlapEvents() == true)
// 		GetWorldTimerManager().SetTimer(TimerHandle_Collision,this,&ASwordActor::SetCollisionFalse,0.3f,false);
// }
//
// void ASwordActor::SetCollisionFalse()
// {
// 	BoxCollisonComp->SetGenerateOverlapEvents(false);
// 	GetWorldTimerManager().ClearTimer(TimerHandle_Collision);
// }
