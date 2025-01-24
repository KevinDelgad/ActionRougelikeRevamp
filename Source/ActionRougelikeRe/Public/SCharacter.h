// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UParticleEmitter;
class USInteractionComponent;
//Forward Declaration
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class USAttributeComponent;
UCLASS()
class ACTIONROUGELIKERE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	//Allows assigning of a Class
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHole;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> Dash;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack")
	UParticleSystem* CastingEmitter;
	
	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComp;
	
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);
	
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_PrimaryAbility;
	FTimerHandle TimerHandle_SecondaryAbility;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	virtual void PostInitializeComponents() override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryAttack_TimeElapsed();
	void PrimaryAttack();
	void PrimaryAbility_TimeElapsed();
	void PrimaryAbility();
	void SecondaryAbility_TimeElapsed();
	void SecondaryAbility();
	void PrimaryInteract();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AbiltyAnim(FTimerHandle TimerHandle, TDelegate<void()>::TMethodPtr<ASCharacter> Func);
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

};
