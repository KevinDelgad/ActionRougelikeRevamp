// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;
	bReplicates = true;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	//ISGameplayInterface::Interact_Implementation(Instigator);
	UE_LOG(LogHAL, Log, TEXT("Instigator Pawn: "));
	bLidOpen = !bLidOpen;

	//RepNotify or ReplicatedUsing only does automatic call for client (Except for on blueprint)
	//Manually Called here to ensure it is done on the server
	OnRep_LidOpened();
	
}

void ASItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpen ? TargetPitch : 0;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties
void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Whenever bLidOpen Changes send to all clients
	DOREPLIFETIME(ASItemChest, bLidOpen);
}