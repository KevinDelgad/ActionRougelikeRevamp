// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupBase_Coin.h"

#include "SGameplayFunctionLibrary.h"

// Sets default values
ASPickupBase_Coin::ASPickupBase_Coin()
{
	CoinValue = 5;
}

void ASPickupBase_Coin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ensure(InstigatorPawn))
	{
		AController* Controller = InstigatorPawn->GetController();
	
		if (ensure(Controller))
		{
			if (USGameplayFunctionLibrary::AddPlayerCredits(Controller, CoinValue))
			{
				HideAndCooldownPowerup();
			}
		}
	}
}


