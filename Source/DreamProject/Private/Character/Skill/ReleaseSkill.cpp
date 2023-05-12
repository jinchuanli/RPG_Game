// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skill/ReleaseSkill.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/Skill/ReleaseSkillProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AReleaseSkill::OnTryCastSpell()
{
	Super::OnTryCastSpell();
}

void AReleaseSkill::InitializeSpellCast()
{
	Super::InitializeSpellCast();
}

void AReleaseSkill::OnSpellCast()
{
	PlayerReference->bUseControllerRotationYaw = true;
	Cast<UCharacterMovementComponent>(PlayerReference->GetMovementComponent())->bOrientRotationToMovement = false;
	Super::OnSpellCast();
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrient,this,&AReleaseSkill::ResetOrient,0.1f,false);
}

void AReleaseSkill::OnSkillNotify()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	
	GetWorld()->SpawnActor<AReleaseSkillProjectile>(
		MagicProjectile,
		PlayerReference->GetMesh()->GetSocketLocation("ProjectileSkill"),
		PlayerReference->GetMesh()->GetSocketRotation("ProjectileSkill"),
		Params);
}

void AReleaseSkill::OnPlaySound()
{
	if(sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,sound,PlayerReference->GetMesh()->GetSocketLocation("ProjectileSkill"));
	}
}

void AReleaseSkill::ResetOrient()
{
	PlayerReference->bUseControllerRotationYaw = false;
	Cast<UCharacterMovementComponent>(PlayerReference->GetMovementComponent())->bOrientRotationToMovement = true;
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrient);
}
