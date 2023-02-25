// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RGideon.h"
#include "E:\UE_4.26\Engine\Source\Runtime\CoreUObject\Public\UObject\ConstructorHelpers.h"
#include "Animation/AnimInstance.h"
#include "Engine/SkeletalMesh.h"

ARGideon::ARGideon()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mash(TEXT("SkeletalMesh'/Game/ParagonGideon/Characters/Heroes/Gideon/Meshes/Gideon.Gideon'"));
	GetMesh()->SetSkeletalMesh(Mash.Object);

	GetMesh()->SetRelativeLocation(FVector(0,0,-88));
	GetMesh()->SetRelativeRotation(FRotator(0,-90.f,0));
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("AnimBlueprint'/Game/BluePrints/Character/Animations/ABP_GIDON.ABP_Gidon_C'"));
	GetMesh()->SetAnimInstanceClass(Anim.Class);
}
