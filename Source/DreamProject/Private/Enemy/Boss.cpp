// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss.h"

#include "SimpleDrawTextFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Character/RBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Other/StaticLibrary.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

	AIController = GetController();
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::OnReceiveDamage(float BaseDamage, EDamageType Type, TSubclassOf<ABaseElement> Element, int CritChance,
	AActor* Attacker, ABaseSkill* Spell)
{
	if(!bDead && !UStaticLibrary::bIsEnemy(Attacker))
	{
		int32 Damage = UStaticLibrary::CaculateFinalDamage(BaseDamage,CritChance,Element,this->BossElement);
		CurrentHealth = FMath::Clamp((CurrentHealth - Damage),0.f,TotalHealth);
		USimpleDrawTextFunctionLibrary::SpawnDrawText(this,GetActorLocation()+FVector3d(0,0,200),FString::FromInt(Damage),FColor::Orange,0,EInfoAnimType::TYPE_DEFAULT);
		UE_LOG(LogTemp, Warning, TEXT("current health %f"),CurrentHealth);
		if(CurrentHealth < 0.5f)
		{
			bDead = true;
			BossDead();
		}
	}
}

void ABoss::CreatDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARBaseCharacter* Character = Cast<ARBaseCharacter>(OtherActor);
	if(Character && OtherComp == Character->GetCapsuleComponent())
	{
		IDamageAbleInterface* DamageAbleInterface = Cast<IDamageAbleInterface>(Character);
		if(DamageAbleInterface)
		{
			DamageAbleInterface->OnReceiveDamage(
			BossDamage,
			DamageType,
			BossElement,
			BossCritChance,
			this,
			nullptr
			);
			UE_LOG(LogTemp, Warning, TEXT("character have attack from boss"));
		}
	}
}

void ABoss::BossDead()
{
	SpawnItem(SpawnItemArray);
	if(DeadAnim)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ARBaseCharacter* Character = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
		Character->IncreaseCurrentExp(Receive);
		PlayAnimMontage(DeadAnim);
		if(AIController)
		{
			AIController->UnPossess();
			AIController->Destroy();
		}
		GetWorldTimerManager().SetTimer(TimerHandle_Destroy,this,&ABoss::DestroyBoss,3.f,false);
	}
}

void ABoss::DestroyBoss()
{
	Destroy();
	GetWorldTimerManager().ClearTimer(TimerHandle_Destroy);
}

void ABoss::SpawnItem(TArray<TSubclassOf<AActor>> Item)
{
	for(TSubclassOf<AActor> TempItem : Item)
	{
		if(TempItem)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AActor* Temp = GetWorld()->SpawnActor<AActor>(TempItem,GetActorLocation(),FRotator::ZeroRotator,Params);
			UStaticMeshComponent* StaticMeshComponent = Temp->FindComponentByClass<UStaticMeshComponent>();
			StaticMeshComponent->SetSimulatePhysics(true);
			SpawnActorArray.Add(Temp);
		}
	}
	if(!SpawnActorArray.IsEmpty())
	{
		GetWorldTimerManager().SetTimer(TimerHandle_ItemDestroy,this,&ABoss::SpawnItemDestroy,10.f,false);
	}
}

void ABoss::SpawnItemDestroy()
{
	for(AActor* Temp : SpawnActorArray)
	{
		Temp->Destroy();
	}
	SpawnActorArray.Empty();
	GetWorldTimerManager().ClearTimer(TimerHandle_ItemDestroy);
}
