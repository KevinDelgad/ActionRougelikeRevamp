// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKERE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float KillValue;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")	
	UEnvQuery* SpawnItemQuery;

	UPROPERTY(EditAnywhere, Category = "Consumable")
	TSubclassOf<AActor> HealthPotion;

	UPROPERTY(EditAnywhere, Category = "Consumable")
	int NumHealthPotions;
	
	UPROPERTY(EditAnywhere, Category = "Consumable")
	TSubclassOf<AActor> Coin;

	UPROPERTY(EditAnywhere, Category = "Consumable")
	int NumCoins;
	
	FTimerHandle TimerHandle_SpawnBots;
	
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UFUNCTION()
	void CoinSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
									  EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void PotionSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
									  EEnvQueryStatus::Type QueryStatus);
	
	UFUNCTION()
	void SpawnConsumable(TSubclassOf<AActor> ClassToSpawn, UEnvQueryInstanceBlueprintWrapper* QueryInstance);
	
	public:
	ASGameModeBase();
	
	virtual void StartPlay() override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	
	UFUNCTION(Exec)
	void KillAll();
};
