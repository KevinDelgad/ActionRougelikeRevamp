// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupBase.h"

// Sets default values
ASPickupBase::ASPickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	RootComponent = MeshComp;

	RechargeTime = 10.0f;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASPickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPickupBase::SetPowerupState(bool bIsActive)
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void ASPickupBase::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	GetWorldTimerManager().SetTimer(RechargeTimerHandle, this, &ASPickupBase::ShowPowerup, RechargeTime);
}

void ASPickupBase::ShowPowerup()
{
	SetPowerupState(true);
}

void ASPickupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	//Implement in Child Functions
}
