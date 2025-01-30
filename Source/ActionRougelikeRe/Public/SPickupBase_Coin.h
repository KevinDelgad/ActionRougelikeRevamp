// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickupBase.h"
#include "GameFramework/Actor.h"
#include "SPickupBase_Coin.generated.h"

UCLASS()
class ACTIONROUGELIKERE_API ASPickupBase_Coin : public ASPickupBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickupBase_Coin();

protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly)
	float CoinValue;
};
