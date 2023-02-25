// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InterationInterface.h"
#include "BaseQuestProp.generated.h"

class UStaticMeshComponent;
UCLASS()
class DREAMPROJECT_API ABaseQuestProp : public AActor,public IInterationInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseQuestProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//道具的静态网格
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComp;
	//交互的组件
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* InteractionWidget;

	//播放特效的组件
	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ParticleSystemComp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere,Category=Info)
	FText Name;

	virtual void OnEnterPlayerRadius(class ARBaseCharacter* Character) override;
	virtual void OnLeavePlayerRadius(class ARBaseCharacter* Character) override;
	virtual void OnIteractWith(class ARBaseCharacter* Character) override;
};
