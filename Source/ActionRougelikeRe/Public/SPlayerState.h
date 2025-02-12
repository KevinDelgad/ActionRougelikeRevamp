// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCreditChanged, AActor*, InstigatorActor ,
	ASPlayerState*, OwningComp ,float, NewCredit, float, Delta);

/**
 * 
 */
UCLASS(ClassGroup=(Custom))
class ACTIONROUGELIKERE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

	protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PlayerData")
	float Credits;

	public:
	ASPlayerState();

	UPROPERTY(BlueprintAssignable)
	FOnCreditChanged OnCreditChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static ASPlayerState* GetAttributes(AController* FromController);
	
	UFUNCTION(BlueprintCallable, Category="PlayerData")
	bool UpdateCredits(AActor* InstigatorActor,float Delta);
};
