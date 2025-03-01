// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;
/**
 * 
 */
UCLASS()
class ACTIONROUGELIKERE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:
	//Allows assigning of a Class
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack")
	UParticleSystem* CastingEmitter;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
	
public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
	USAction_ProjectileAttack();
};
