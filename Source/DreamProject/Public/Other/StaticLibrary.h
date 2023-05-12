// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StaticLibrary.generated.h"

class ABaseElement;
/**
 * 
 */
UCLASS()
class DREAMPROJECT_API UStaticLibrary : public UObject
{
	GENERATED_BODY()
public:
	static float CaculateFinalDamage(float BaseDamage,int CritChance,TSubclassOf<ABaseElement> AttackerElement,TSubclassOf<ABaseElement> DefenderElement);

	//110   判断是否是敌人
	static bool bIsEnemy(AActor* Actor);

	//模板
	template<typename TEnum>
	static FORCEINLINE FString GetEnumValueAsString(const FString& Name,TEnum Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE,*Name,true);
		if(!EnumPtr)
		{
			return FString("InValid"); 
		}
		else
		{
			FString TempString = EnumPtr->GetNameByValue((int64)Value).ToString();
			TempString.RemoveFromStart(Name+"::");
			return TempString;
		}
	};

	template<typename TEnum>
	static FORCEINLINE TArray<TEnum> EnumGetList(const FString& Name);  //想要类进行调用前面必须添加static
};

template <typename TEnum>
TArray<TEnum> UStaticLibrary::EnumGetList(const FString& Name)
{
	TArray<TEnum> Result;
	UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE,*Name,true);
	for(int i = 0; i < pEnum->GetMaxEnumValue();i++)
	{
		if(pEnum->IsValidEnumValue(i))
		{
			Result.Add(static_cast<TEnum>(i));
		}
	}
	return Result;
}
