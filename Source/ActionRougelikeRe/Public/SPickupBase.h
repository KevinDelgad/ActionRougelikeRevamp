// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickupBase.generated.h"

class UMeshComponent;

UCLASS()
class ACTIONROUGELIKERE_API ASPickupBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickupBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(VisibleAnywhere)
	UMeshComponent* MeshComp;
	FTimerHandle RechargeTimerHandle;
	UPROPERTY(EditAnywhere)
	float RechargeTime;
	
	void SetPowerupState(bool bIsActive);
	void HideAndCooldownPowerup();
	void ShowPowerup();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
