// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

class UParticleEmitter;
/**
 * 
 */
UCLASS()
class ACTIONROUGELIKERE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ImpactParticle;
	
	FTimerHandle AliveTimer;
	FTimerHandle TeleportTimer;
	
	void TeleportAnim();
	void Teleport();
	UFUNCTION()
	void ImmediateTeleport(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;
	
public:
	ASDashProjectile();
	
};
