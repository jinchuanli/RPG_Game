// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/ItemStaff.h"
#include "Bow.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API ABow : public AItemStaff
{
	GENERATED_BODY()
	ABow();
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* BowMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BaseVelocity = 1000.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxVelocity = 5000.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ChargingStep = 10.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ReloadTime = 1.0f;
	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	// USceneComponent* Scene;
public:
	FORCEINLINE float GetBaseVelocity(){ return  BaseVelocity; };
	FORCEINLINE float GetMaxVelocity(){ return  MaxVelocity; };
	FORCEINLINE float GetChargingStep(){ return  ChargingStep; };
	FORCEINLINE float GetReloadTime(){ return  ReloadTime; };

	FORCEINLINE USkeletalMeshComponent* GetBowMesh() { return BowMesh; };

	virtual void OnUsed() override;
};
