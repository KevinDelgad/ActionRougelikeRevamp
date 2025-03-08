// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComponent = CreateDefaultSubobject<USActionComponent>("ActionComp");
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	TimeToHitParamName = "TimeToHit";

	AlertDuration = 2.0f;
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                    float Delta)
{
	if (Delta <	0.0f)
	{

		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
				
			}
		}
		
		
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());
		
		if (NewHealth <= 0.0f)
		{
			//Stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				//BrainComp is baseclass of behavior tree comp
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			//Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			//LifeSpan
			SetLifeSpan(10.0f);
		}
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
}

void ASAICharacter::ClearWidget()
{
	AlertWidgetInstance->RemoveFromParent();
	AlertWidgetInstance = nullptr;
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		if (NewTarget != AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor"))
		{
			if (AlertWidgetInstance == nullptr && ensure(AlertWidgetClass))
			{
				AlertWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), AlertWidgetClass);
			}

			if (AlertWidgetInstance)
			{
				AlertWidgetInstance->AttachedActor = this;
			
				if (!AlertWidgetInstance->IsInViewport())
				{
					AlertWidgetInstance->AddToViewport();
					FTimerHandle AlertEnd;

					GetWorldTimerManager().SetTimer(AlertEnd, this, &ASAICharacter::ClearWidget, AlertDuration, false);
				}
			}
		}
		
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);

		//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
		
	}
}
