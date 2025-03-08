// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupBase_Ability.h"

#include "SActionComponent.h"

void ASPickupBase_Ability::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ensure(InstigatorPawn))
	{
		USActionComponent* ActionComponent = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));

		if (ensure(ActionComponent && ActionToGrant))
		{
			if (!(ActionComponent->CheckForAction(FName(ActionToGrant->GetName()))))
			{
				ActionComponent->AddAction(InstigatorPawn, ActionToGrant);
				HideAndCooldownPowerup();
			}
		}
	}
}
