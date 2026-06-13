// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoul/Items/ChangeMapTrigger.h"
#include <Components/BoxComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <Kismet/GameplayStatics.h>
#include "../DrakSoulGameInstance.h"

// Sets default values
AChangeMapTrigger::AChangeMapTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ChangeMapTrigger"));
	BoxTrigger->SetupAttachment(RootComponent);

	Nia = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TriggerNia"));
	Nia->SetupAttachment(RootComponent);

	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AChangeMapTrigger::ChangeMapFunc);
}

// Called when the game starts or when spawned
void AChangeMapTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChangeMapTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChangeMapTrigger::ChangeMapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UDrakSoulGameInstance* GI = Cast<UDrakSoulGameInstance>(GetGameInstance()))
	{
		GI->IsInFightMap = true;
	}
	UGameplayStatics::OpenLevel(GetWorld(), "FightMap");
}

