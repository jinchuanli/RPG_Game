// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MiniCam.h"
#include "Character/RBaseCharacter.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"

AMiniCam::AMiniCam()
{
	GetCaptureComponent2D()->SetRelativeRotation(FRotator(-90.f,0.f,0.f));
}

void AMiniCam::BeginPlay()
{
	Super::BeginPlay();
	//获得主角
	Character = Cast<ARBaseCharacter>(UGameplayStatics::GetPlayerPawn(this,0));
	
	SetActorLocation(FVector(Character->GetActorLocation().X,Character->GetActorLocation().Y,Character->GetActorLocation().Z+1000.f));
}

void AMiniCam::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	OutSideTick();
	//GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,"Location");
}

void AMiniCam::OutSideTick()
{
	//FVector TargetLocation = FVector(Character->GetActorLocation().X,Character->GetActorLocation().Y,Character->GetActorLocation().Z+1000.f);
	FVector TargetLocation = Character->GetActorLocation() + FVector(0,0,1000.f);
	//绕着Yaw轴旋转
	// FRotator YawRotator = Character->GetActorRotation();
	// FRotator TargetRotator = FRotator(0,YawRotator.Yaw,0);
	SetActorLocation(TargetLocation);
	// SetActorRotation(TargetRotator);
	// GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,"Location");
	
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,"No Location");
	// }
}
