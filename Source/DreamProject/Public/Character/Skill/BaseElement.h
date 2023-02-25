// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementInfo.h"
#include "GameFramework/Actor.h"
#include "BaseElement.generated.h"

UCLASS()
class DREAMPROJECT_API ABaseElement : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//结构体都只能用引入头文件的方式
	UPROPERTY(EditAnywhere,Category=ElementInfo)
	FElementInfo ElementInfo;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
