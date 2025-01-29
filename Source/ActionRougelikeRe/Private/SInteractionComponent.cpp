// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDrawDebug"), true, TEXT("Enabling Debug Lines for Interact Component"), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	//Add Vector to ensure looking the correct direction plus constant to determine distance
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f);
	
	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	//Sweeping is taking a sphere and moves it virtually from start to end to find overlaps and first hit

	TArray<FHitResult> Hits;
	//FQuat::Identity means default or empty rotation (No Rotatiion)
	//Quat is a more complex rotator which has more info avaliable to avoid locking (Better Version of Rotator)

	float Radius = 30.0f;
	
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity,
		ObjectQueryParams, Shape);
	
	FColor LineColor = bBlockingHit ? FColor::Red : FColor::Green;
	
	for (FHitResult Hit: Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false,2.0f);
		}
		
		if(AActor* HitActor = Hit.GetActor())
		{
			//Ensure the object which raytrace hits uses the interface
			//U is used to check if something implements it as opposed to I
			if(HitActor->Implements<USGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				
				//Execute the target with the syntax Execute_FunctionName()
				//First parameter cannot be null as it would crash trying to find function to execute
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
	}
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2, 0, 2.0f);
	}
}