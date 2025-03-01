// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SGameplayFunctionLibrary.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/Tasks/BTTask_RunEQSQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enabling spawning of bots Via Timer"), ECVF_Cheat);



ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	NumCoins = 6;
	NumHealthPotions = 6;
	
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	//Smarter Approach, only run query once and spawn at locations from array while removing the location
	for (int curPotions = 0; curPotions <  NumHealthPotions; curPotions++)
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnItemQuery, this,EEnvQueryRunMode::RandomBest5Pct, nullptr);
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::PotionSpawnQueryCompleted);
	}
	
	for (int curCoins = 0; curCoins <  NumHealthPotions; curCoins++)
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnItemQuery, this,EEnvQueryRunMode::RandomBest5Pct, nullptr);
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::CoinSpawnQueryCompleted);
	}
	
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if ensure(Controller)
	{
		Controller->UnPossess();

		//Teleports player to spawn area
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::CoinSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Coin Consumable EQS Query Failed"));
		return;
	}

	SpawnConsumable(Coin, QueryInstance);
	
}

void ASGameModeBase::PotionSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                               EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Potion Consumable EQS Query Failed"));
		return;
	}

	SpawnConsumable(HealthPotion, QueryInstance);
}

void ASGameModeBase::SpawnConsumable(TSubclassOf<AActor> ClassToSpawn, UEnvQueryInstanceBlueprintWrapper* QueryInstance)
{

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);

	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	AController* Controller = Killer->GetInstigatorController();
	if (Controller)
	{
		USGameplayFunctionLibrary::AddPlayerCredits(Controller, 5);
	}
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);

		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // TODO: Pass in player? for kill credit
		}
	}
}


void ASGameModeBase::SpawnBotTimerElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawning disabled via cvar. 'CVarSpawnBots'. "))
		return;
	}
	//TActor iterator, better version of get all actors of class by getting pointer to all instances
	//of the target in the world in this case ASAICharacter
	//Grabs any instance of a particular class in the current level
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);

		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrOfAliveBots)
	
	float MaxBotCount = 10.0f;
	
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At max bot count: %f, skipping bot spawn."), MaxBotCount)
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this,EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensureMsgf(QueryInstance, TEXT("Spawn Bot Query has not been set. Please set Spawn Bot Query")))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                      EEnvQueryStatus::Type QueryStatus)
{
	// Gives location of where to spawn bot

	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn Bot EQS Query Failed"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

