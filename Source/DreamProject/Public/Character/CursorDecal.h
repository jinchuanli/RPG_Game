// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ShapeComponent.h"
#include "CursorDecal.generated.h"

UCLASS()
class DREAMPROJECT_API ACursorDecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACursorDecal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UDecalComponent *CursorToWorld;
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent *DefaultRootComponent;
	
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent *BoxComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//碰撞检测
	void NotifyActorBeginOverlap(AActor *OtherActor) override;
	
	//对于AddDynamic都要添加UFUNCTION()才有用
	// UFUNCTION()
	// void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
