// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction_Effect.h"
#include "MySAction_EffectThorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKERE_API UMySAction_EffectThorns : public USAction_Effect
{
	GENERATED_BODY()

public:
	UMySAction_EffectThorns();
	
protected:

	UPROPERTY(EditDefaultsOnly)
	float ThornPercentage;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta);
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
};
