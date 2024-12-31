// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SBlackholeProjectile.generated.h"

class URadialForceComponent;
/**
 * 
 */
UCLASS()
class ACTIONROUGELIKERE_API ASBlackholeProjectile : public ASProjectileBase
{
	
GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBlackholeProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void DestroySelf();
UFUNCTION()
void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
UFUNCTION()
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere)
	USphereComponent* BlackHole;
	
UPROPERTY(EditAnywhere)
	URadialForceComponent* PullForceComponent;

	FTimerHandle DestroyTimerHandle;

};
