// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoul/Items/PickBase.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>
#include "../Character/SoulPlayerCharacter.h"
#include "../DrakSoulGameInstance.h"

// Sets default values
APickBase::APickBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickItem"));
	RootComponent = PickItemMesh;

	PickNia = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PickNiagara"));
	PickNia->SetupAttachment(RootComponent);

	PickColl = CreateDefaultSubobject<USphereComponent>(TEXT("PickCollision"));
	PickColl->SetupAttachment(RootComponent);

	ClothType = EClothType::NONE;
	SK_PickItem = nullptr;
}

// Called when the game starts or when spawned
void APickBase::BeginPlay()
{
	Super::BeginPlay();
	PickColl->OnComponentBeginOverlap.AddDynamic(this,&APickBase::PickItemToPlayer);
}

// Called every frame
void APickBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickBase::PickItemToPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASoulPlayerCharacter* Player = Cast<ASoulPlayerCharacter>(OtherActor))
	{
		if (UDrakSoulGameInstance* GI = Cast<UDrakSoulGameInstance>(GetGameInstance()))
		{
			switch (ClothType)
			{
			case EClothType::HAT:
				Player->Hat->SetSkeletalMeshAsset(SK_PickItem);
				GI->Hat = SK_PickItem;
				K2_DestroyActor();
				break;
			case EClothType::TOPCLOTH:
				Player->GetMesh()->SetSkeletalMeshAsset(SK_PickItem);
				GI->TopCloth= SK_PickItem;
				K2_DestroyActor();
				break;
			case EClothType::BOTCLOTH:
				Player->Pants->SetSkeletalMeshAsset(SK_PickItem);
				GI->BotCloth = SK_PickItem;
				K2_DestroyActor();
				break;
			case EClothType::SHOES:
				Player->Shoes->SetSkeletalMeshAsset(SK_PickItem);
				GI->Shoes = SK_PickItem;
				K2_DestroyActor();
				break;
			default:
				break;
			}
		}
	}
}

