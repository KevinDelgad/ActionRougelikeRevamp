// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class URadialForceComponent;
class UCapsuleComponent;

UCLASS()
class ACTIONROUGELIKERE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();


protected:
	UFUNCTION(BlueprintCallable)
	void Explode();

	//Must be Exposed to UFUNCTION
	UFUNCTION(BlueprintCallable)
	void BarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
				   FVector NormalImpulse, const FHitResult& Hit);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForce;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
