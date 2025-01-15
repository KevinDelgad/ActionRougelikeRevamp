// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTaskNode_HealSelf.h"

#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTaskNode_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
				float MaxHealth = AttributeComponent->GetMaxHealth();
				return AttributeComponent->ApplyHealthChange(MaxHealth) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
			}
		}
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
