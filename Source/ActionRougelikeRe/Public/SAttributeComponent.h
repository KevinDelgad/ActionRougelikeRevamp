// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor ,
	USAttributeComponent*, OwningComp ,float, NewHealth, float, Delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor ,
	USAttributeComponent*, OwningComp ,float, NewRage, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGELIKERE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	// Sets default values for this component's properties
	USAttributeComponent();

protected:
	// EditAnywhere	edit in BP editor and per-instance in level.
	// VisibleAnywhere 'read-only' in editor and level. (Use for Components)
	// EditDefaultsOnly hide variable per-instance, edit in BP editor only
	// VisibleDefaultsOnly 'read-only' access for variable, only in BP editor (uncommon)
	// EditInstancesOnly - Allow only editing of instance (e.g. when placed in level)
	//--
	// BlueprintReadOnly read-only in the Blueprint scripting (does not affect 'details' -panel)
	// BlueprintReadWrite  read-write access in Blueprints
	//--
	// Category = "" - display only for detail panels and blueprint context menu.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes")
	float HealthMax;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Rage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float RageMax;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float RageConversionRate;
	// HealthMax, Stamina, Strength

	//Use Multicast for transient Events (Things happening in the moment but dont change the on going state. EX: Barrel Explosion)
	//Rep Notifies are useful for state Changes (Ongoing)/

	//@FIXME: Mark as unreliable once we moved the 'state' of our character
	//When marked as unreliable it takes relevancy into account
	//Relevancy - How relevate a player is to recieve data? Are they close enough to be considered relevant enough?
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);
	
public:

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor,float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyMaxHealthChange(float Delta);
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	float GetHealth() const;
};
