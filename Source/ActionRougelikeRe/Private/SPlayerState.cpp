// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

ASPlayerState::ASPlayerState()
{
	Credits = 0;
}

ASPlayerState* ASPlayerState::GetAttributes(AController* FromController)
{
	if (FromController)
	{
		return FromController->GetPlayerState<ASPlayerState>();
	}

	return nullptr;
}

bool ASPlayerState::UpdateCredits(AActor* InstigatorActor, float Delta)
{
	float NewCredit = Credits + Delta;

	if (NewCredit < 0)
	{
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Player %s new credit value: %f"), *GetNameSafe(InstigatorActor), NewCredit);
	
	Credits += Delta;
	return true;
}
