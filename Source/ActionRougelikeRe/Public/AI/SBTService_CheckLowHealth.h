// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKERE_API USBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector IsLowHealth;

	UPROPERTY(EditAnywhere, Category = "AI")
	float LowHealthValue;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
