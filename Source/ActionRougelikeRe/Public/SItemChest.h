// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROUGELIKERE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

	void Interact_Implementation(APawn* InstigatorPawn);
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

	UPROPERTY(EditAnywhere)
	float TargetPitch;

protected:

	// RepNotify or ReplicatedUsing binds the changing of a variable to a particular function to be called
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadOnly) // RepNotify
	bool bLidOpen;

	UFUNCTION()
	void OnRep_LidOpened();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
