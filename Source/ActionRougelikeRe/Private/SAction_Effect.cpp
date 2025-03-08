// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Effect.h"

#include "SActionComponent.h"

USAction_Effect::USAction_Effect()
{
	Duration = 3.0f;
	Period = 1.0f;
	bAutoStart = true;
}

void USAction_Effect::StartAction_Implementation_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,FName("StopAction"), Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,FName("ExecutePeriodicEvent"), Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
		
	}
}

void USAction_Effect::StopAction_Implementation_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Ending Effect!"));
	//Check if Timer Was about to run (Back Up for race conditions)
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEvent(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	USActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}

void USAction_Effect::ExecutePeriodicEvent_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Executing Effect!"));
}
