// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_Effect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKERE_API USAction_Effect : public USAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Action")
	void StopAction_Implementation(AActor* Instigator) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;


	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEvent(AActor* Instigator);
public:
	USAction_Effect();
};
