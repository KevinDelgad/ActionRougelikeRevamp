// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
	Rage = 0;
	RageConversionRate = 2;
	RageMax = 100;
}


bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor ,float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if(ActualDelta < 0.0f)
	{
		ApplyRageChange(InstigatorActor, abs(ActualDelta));
		
		if(Health == 0.0f)
		{
			ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	
	return ActualDelta != 0;
}

bool USAttributeComponent::ApplyMaxHealthChange(float Delta)
{
	HealthMax += Delta;

	return true;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	UE_LOG(LogTemp, Display, TEXT("Delta = %f"), Delta);
	
	float ConvertedRage = Delta * RageConversionRate;

	Rage = FMath::Clamp(Rage + ConvertedRage, 0, RageMax);

	FString DebugMsg = "Current Rage: " + FString::FromInt(Rage);

	OnRageChanged.Broadcast(InstigatorActor, this, Rage, Delta);
	
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, DebugMsg);
	
	return true;
}


bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float USAttributeComponent::GetMaxHealth() const
{
	return HealthMax;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->GetComponentByClass<USAttributeComponent>();
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}

	return false;
}
