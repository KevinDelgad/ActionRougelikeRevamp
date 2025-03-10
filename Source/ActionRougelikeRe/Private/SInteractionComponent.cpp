// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDrawDebug"), false, TEXT("Enabling Debug Lines for Interact Component"), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	CollisionChannel = ECollisionChannel::ECC_WorldStatic;
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

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if(MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
	// ...
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	//Add Vector to ensure looking the correct direction plus constant to determine distance
	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);
	
	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	//Sweeping is taking a sphere and moves it virtually from start to end to find overlaps and first hit

	TArray<FHitResult> Hits;
	//FQuat::Identity means default or empty rotation (No Rotatiion)
	//Quat is a more complex rotator which has more info avaliable to avoid locking (Better Version of Rotator)
	
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity,
		ObjectQueryParams, Shape);
	
	FColor LineColor = bBlockingHit ? FColor::Red : FColor::Green;

	//Clear Ref Before Trying tp fill
	FocusedActor = nullptr;
	
	for (FHitResult Hit: Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false,0.0f);
		}
		
		if(AActor* HitActor = Hit.GetActor())
		{
			//Ensure the object which raytrace hits uses the interface
			//U is used to check if something implements it as opposed to I
			if(HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstace == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstace = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstace)
		{
			DefaultWidgetInstace->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstace->IsInViewport())
			{
				DefaultWidgetInstace->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstace)
		{
			DefaultWidgetInstace->RemoveFromParent();
		}
	}
	
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 0, 0, 2.0f);
	}
}


void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if(InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("No FocusedActor"));
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	//Execute the target with the syntax Execute_FunctionName()
	//First parameter cannot be null as it would crash trying to find function to execute
	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}
