// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/CiYuanZhan.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy/NormalEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/Boss.h"

void ACiYuanZhan::OnSkillNotify()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),initiativeEffect,PlayerReference->GetActorLocation(),FRotator::ZeroRotator);
	if(StartSound)
		UGameplayStatics::PlaySoundAtLocation(this,StartSound,GetActorLocation());

	TArray<FHitResult> HitResults;

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(GetCurrentStage().Range);
	if(GetWorld()->SweepMultiByChannel(HitResults,
		PlayerReference->GetActorLocation(),
		PlayerReference->GetActorLocation(),
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
						for(int32 i = 0; i < NumOfDamage; i++)
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
							for(int32 i = 0; i < NumOfDamage; i++)
								DamageAbleInterface->OnReceiveDamage(GetCurrentStage().Damage,GetCurrentStage().DamageType,GetSkillInfo().Element,
									GetCurrentStage().CriticalChance,PlayerReference,this);
						}
					}
				}
				
			}
		}
	}
}

