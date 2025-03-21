// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USWorldUserWidget;
class USAttributeComponent;
class UPawnSensingComponent;
class UUserWidget;
class USActionComponent;

UCLASS()
class ACTIONROUGELIKERE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	FName TimeToHitParamName;
	
	void SetTargetActor(AActor* NewTarget);
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USActionComponent* ActionComponent;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<class USWorldUserWidget> AlertWidgetClass;

	UPROPERTY()
	USWorldUserWidget* AlertWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float AlertDuration;
	
	void ClearWidget();
	
	virtual void PostInitializeComponents() override;
};
