// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryStruct.h"
#include "GameFramework/Actor.h"
#include "Interface/InterationInterface.h"
#include "BaseItem.generated.h"

UCLASS()
class DREAMPROJECT_API ABaseItem : public AActor,public IInterationInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();
	//在蓝图中给物品的基本信息赋值
	UPROPERTY(EditAnywhere,Category=ItemInfo)
	FItemInfo ItemInfo;

	UPROPERTY(EditAnywhere,Category=ItemInfo)
	int Amount;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollisonComp;
	//显示背包样式
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComp;

	//创建交互
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* InterationWidgetComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//继承交互的接口
	virtual void OnEnterPlayerRadius(class ARBaseCharacter* Character) override;
	virtual void OnLeavePlayerRadius(class ARBaseCharacter* Character) override;
	virtual void OnIteractWith(class ARBaseCharacter* Character) override;


};
