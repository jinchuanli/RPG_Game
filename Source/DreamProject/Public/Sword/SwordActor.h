// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class DREAMPROJECT_API ASwordActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UStaticMeshComponent* Weapon;
	ASwordActor();
	class ARBaseCharacter* PlayerRef;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category=Attack)
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//void OnOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// FTimerHandle TimerHandle_Collision;
	// UFUNCTION(BlueprintCallable,Category=Attack)
	// void SetCollision();
	// void SetCollisionFalse();
};
