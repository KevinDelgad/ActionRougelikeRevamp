// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickupBase.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

UCLASS()
class ACTIONROUGELIKERE_API ASHealthPotion : public ASPickupBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHealthPotion();

protected:

	UPROPERTY(EditDefaultsOnly)
	float HealAmount;

	UPROPERTY(EditDefaultsOnly)
	float Cost;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
};
