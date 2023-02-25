// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/SkillNova.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy/NormalEnemy.h"
#include "Kismet/KismetSystemLibrary.h"

void ASkillNova::OnSkillNotify()
{
	//Super::OnSkillNotify();//写了super就是会执行父类的类容
	//生成的位置
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ParticleSystem,PlayerReference->GetMesh()->GetSocketLocation("Foot_L"),
		PlayerReference->GetActorRotation());

	TArray<FHitResult> FHitResults;

	FCollisionShape CollisonSphere = FCollisionShape::MakeSphere(GetCurrentStage().Range);
	if(GetWorld()->SweepMultiByChannel(FHitResults,PlayerReference->GetActorLocation(),PlayerReference->GetActorLocation(),
		FQuat::Identity,ECC_Visibility,CollisonSphere))
	{
		//画碰撞体的辅助线
		UKismetSystemLibrary::DrawDebugSphere(this,PlayerReference->GetActorLocation(),GetCurrentStage().Range,12,
			FLinearColor::Red,3.f,2.f);
		for(auto FHitResult : FHitResults)
		{
			if(FHitResult.GetActor())
			{
				ANormalEnemy* Enemy = Cast<ANormalEnemy>(FHitResult.GetActor());
				if(Enemy)
				{
					//造成伤害,调用造成伤害的接口
					IDamageAbleInterface* DamageAbleInterface = Cast<IDamageAbleInterface>(Enemy);
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
