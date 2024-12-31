// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetSimulatePhysics(true);
	RootComponent = StaticMesh;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(RootComponent);
	
	RadialForce->SetAutoActivate(false);
	
	RadialForce->ImpulseStrength = 5000.0f;
	RadialForce->bIgnoreOwningActor = true;
	RadialForce->bImpulseVelChange = true;
	
	RadialForce->AddCollisionChannelToAffect(ECC_WorldDynamic);
	
}

void ASExplosiveBarrel::Explode()
{
	RadialForce->FireImpulse();
	//Use Text Macro as it converts to correct type

}

void ASExplosiveBarrel::BarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& Hit)
{
	
	UE_LOG(LogTemp, Log, TEXT("Something Hit Explosive Barrel!"));
	//GetNameSafe Ensures it's not null/Handles gracefully if is null
	//* Converts the type from FString to the expected Type of Char Array

	//%s = String
	//%f = Float
	// logs: "Other Actor: MyActor_1, at game time: 124.4"
	UE_LOG(LogTemp, Warning, TEXT("OtherActor : %s at game time: %f"),
		*GetNameSafe(OtherActor), GetWorld()->TimeSeconds)

	FString CombinedString = FString::Printf(TEXT("Hit at Location: %s"), *Hit.ImpactPoint.ToString());
	
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
	Explode();
}



// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

}

//Always Bind Delegates (Events) to this
void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::BarrelHit);
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

