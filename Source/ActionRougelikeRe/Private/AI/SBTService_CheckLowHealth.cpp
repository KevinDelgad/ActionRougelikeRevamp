// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	
	if ensureMsgf(BlackboardComp, TEXT("No BlackboardComponent is set for the AI. Please set a blackboard component"))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController))
		{
			APawn* AIPawn = MyController->GetPawn();

			if (ensure(AIPawn))
			{
				USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

				if (ensureMsgf(AttributeComponent, TEXT("Target AI %s does not have Attribute Component! Please Add Attribute Component to Target AI."),
					*(AIPawn->GetName())))
				{
					bool bIsLowHealth = AttributeComponent->GetHealth() < LowHealthValue;
					BlackboardComp->SetValueAsBool(IsLowHealth.SelectedKeyName, bIsLowHealth);
				}
			}
		}
	}
}
