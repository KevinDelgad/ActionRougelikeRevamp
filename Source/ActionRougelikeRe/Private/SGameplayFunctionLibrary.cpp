// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComponent)
	{
		return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;	
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp =  HitResult.GetComponent();

		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			//Direction = Target - Origin
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			//Normalize to properly tunr into a direction vector
			Direction.Normalize();
			
			HitComp->AddImpulseAtLocation(Direction * 300000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}

	return false;
}

bool USGameplayFunctionLibrary::AddPlayerCredits(AController* TargetController, float CreditAmount)
{
	if (ASPlayerState* PlayerState = ASPlayerState::GetAttributes(TargetController))
	{
		return PlayerState->UpdateCredits(TargetController, CreditAmount);
	}
	return false;
}

bool USGameplayFunctionLibrary::SubtractPlayerCredits(AController* TargetController, float CreditAmount)
{
	return AddPlayerCredits(TargetController, -CreditAmount);
}
