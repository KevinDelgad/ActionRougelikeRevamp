// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class USActionComponent;
class UWorld;
/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROUGELIKERE_API USAction : public UObject
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category="Action")
	USActionComponent* GetOwningComponent() const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;
	
public:
	UFUNCTION(BlueprintCallable, Category="Action")
	bool IsRunning() const;

	UPROPERTY(EditDefaultsOnly, Category="Action")
	bool bAutoStart;
	
	UFUNCTION(BlueprintNativeEvent, Category="Action")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);
	
	//Using FName cause its hashed (One time assignment) which allows for fast comparisons
	UPROPERTY(EditAnywhere, Category = "Action")
	FName ActionName;

	UWorld* GetWorld() const override;

	
};
