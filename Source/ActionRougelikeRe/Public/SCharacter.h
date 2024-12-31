// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USInteractionComponent;
//Forward Declaration
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
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

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;
	
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_PrimaryAbility;
	FTimerHandle TimerHandle_SecondaryAbility;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryAttack_TimeElapsed();
	void PrimaryAttack();
	void PrimaryAbility_TimeElapsed();
	void PrimaryAbility();
	void SecondaryAbility_TimeElapsed();
	void SecondaryAbility();
	void PrimaryInteract();
	FRotator GetProjectileRotationToCrosshair(FVector SpawnLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AbiltyAnim(FTimerHandle TimerHandle, TDelegate<void()>::TMethodPtr<ASCharacter> Func);


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
