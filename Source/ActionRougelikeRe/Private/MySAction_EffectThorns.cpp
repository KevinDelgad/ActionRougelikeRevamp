// Fill out your copyright notice in the Description page of Project Settings.


#include "MySAction_EffectThorns.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SGameplayFunctionLibrary.h"

UMySAction_EffectThorns::UMySAction_EffectThorns()
{
	Duration = 0.0f;
	Period = 0.0f;
	ThornPercentage = .20f;
}

void UMySAction_EffectThorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	AActor* OwningPlayer = Cast<AActor>(OwningComp->GetOwner());

	//Early out if Player Harmed Self
	if (InstigatorActor == OwningPlayer) {return;}
	
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(InstigatorActor);

	if (AttributeComponent && Delta < 0.0f)
	{
		float ThornDamage = abs(Delta * ThornPercentage);
		USGameplayFunctionLibrary::ApplyDamage(OwningPlayer, InstigatorActor, ThornDamage);
	}
}

void UMySAction_EffectThorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (AActor* OwningPlayer = Cast<AActor>(GetOwningComponent()->GetOwner()))
	{
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(OwningPlayer);

		if (ensure(AttributeComponent))
		{
			AttributeComponent->OnHealthChanged.AddDynamic(this, &UMySAction_EffectThorns::OnHealthChanged);
		}
	}
}
