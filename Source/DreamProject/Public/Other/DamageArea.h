// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageArea.generated.h"

UCLASS()
class DREAMPROJECT_API ADamageArea : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ADamageArea();
	//103
	UPROPERTY(VisibleAnywhere,Category=Component)
	class UParticleSystemComponent* ParticleSystem;
	UPROPERTY(VisibleAnywhere,Category=Component)
	class UBoxComponent* BoxComponent;

	//104
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//105
	void ApplayAreaDamage();
	FTimerHandle TimerHandle_Damage;  //时间句柄
	class ARBaseCharacter* Player;
};
