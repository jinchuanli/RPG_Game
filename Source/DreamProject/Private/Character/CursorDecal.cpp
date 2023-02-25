// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CursorDecal.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Components/BoxComponent.h"

// Sets default values
ACursorDecal::ACursorDecal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//设置蓝图
	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	RootComponent = DefaultRootComponent;
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>(TEXT("CursorToWorld"));
	CursorToWorld->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	//设置蓝图位置
	CursorToWorld->DecalSize = FVector(16.0f,32.0f,32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.f,0.f,0.f).Quaternion());

	//设置大小
	BoxComponent->SetRelativeRotation(FRotator(90.f,0.f,0.f).Quaternion());
	BoxComponent->SetBoxExtent(FVector(20.f,34.f,34.f));

	//绑定在组件上的碰撞
	//BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ACursorDecal::OnOverlap);
	
	//设置初始值
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("/Game/Texture/M_Cursor_Decal"));
	if(DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
}

// Called when the game starts or when spawned
void ACursorDecal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACursorDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//重写父类  针对actor来说的碰撞检测的方式
void ACursorDecal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if(OtherActor == GetOwner())
	{
		Destroy();
	}
}

//针对组件来说的碰撞
// void ACursorDecal::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if(OtherActor == GetOwner())
// 	{
// 		Destroy();
// 	}
// }

