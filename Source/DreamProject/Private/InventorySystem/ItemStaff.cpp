// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemStaff.h"
#include "Enemy/NormalEnemy.h"
#include "Character/RBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/Boss.h"
#include "InventorySystem/Inventory.h"

void AItemStaff::OnUsed()
{
	if(!InventoryRef->PlayerCharacter->EquipItem_Weapon(this))
	{
		Destroy();
	}
}

void AItemStaff::CreatDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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
				Damage,
				DamageType,
				BaseElement,
				CritChance,
				this,
				nullptr);
			UE_LOG(LogTemp, Warning, TEXT("Attack to monster"));
		}
	}
	else
	{
		ABoss* Boss = Cast<ABoss>(OtherActor);
		if(Boss && OtherComp == Boss->GetCapsuleComponent())
		{
			IDamageAbleInterface* DamageAbleInterface = Cast<IDamageAbleInterface>(Boss);
			if(DamageAbleInterface)
			{
				DamageAbleInterface->OnReceiveDamage(
				Damage,
				DamageType,
				BaseElement,
				CritChance,
				this,
				nullptr);
				UE_LOG(LogTemp, Warning, TEXT("Attack to Boss"));
			}
		}
	}
}

bool AItemStaff::EuqlEnum(FString Text)
{
	FString Temp;
	switch (WeaponType)
	{
	case EWeaponType::Sword:
		{
			Temp = FString("Sword");
			break;
		}
	case EWeaponType::Hammer:
		{
			Temp = FString("Hammer");
			break;
		}
	case EWeaponType::BowAndArrow:
		{
			Temp = FString("BowAndArrow");
			break;
		}
	default:
		break;	
	}
	if(Temp.Equals(Text))
	{
		return true;
	}
	else
	{
		return false;
	}
}

