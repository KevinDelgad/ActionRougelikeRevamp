// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKERE_API USGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Gameplay")
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);

	UFUNCTION(BlueprintPure, Category = "Gameplay")
	//When passing something as reference, it will become and output pin rather than an input pin. We Make it input with const
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);

	UFUNCTION(BlueprintPure, Category = "Gameplay")
	static bool AddPlayerCredits(AController* TargetController, float CreditAmount);

	UFUNCTION(BlueprintPure, Category = "Gameplay")
	static bool SubtractPlayerCredits(AController* TargetController, float CreditAmount);
};
