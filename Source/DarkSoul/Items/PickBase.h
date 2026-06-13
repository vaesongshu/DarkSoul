// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../SoulEnumType.h"
#include "PickBase.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;
class UNiagaraComponent;
class USphereComponent;
UCLASS()
class DARKSOUL_API APickBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Items)
	UStaticMeshComponent* PickItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	USkeletalMesh* SK_PickItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	EClothType ClothType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	UNiagaraComponent* PickNia;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	USphereComponent* PickColl;

	UFUNCTION()
	void PickItemToPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
