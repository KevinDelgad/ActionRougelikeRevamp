// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SPickupBase.h"
#include "SPickupBase_Ability.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKERE_API ASPickupBase_Ability : public ASPickupBase
{
	GENERATED_BODY()

	protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditAnywhere, Category = "Action")
	TSubclassOf<USAction> ActionToGrant;
	
};
