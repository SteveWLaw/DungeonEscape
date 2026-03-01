// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MoverActor)
	{
		Mover = MoverActor->FindComponentByClass<UMover>();
		if (Mover)
		{
			UE_LOG(LogTemp, Warning, TEXT("Mover component found in %s"), *MoverActor->GetName());
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Mover component not found in %s"), *MoverActor->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MoverActor is not set in %s"), *GetOwner()->GetName());
	}

	if (IsPressurePlate)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd);
	}
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTriggerComponent::Trigger(bool NewTriggerValue)
{
	IsTriggered = NewTriggerValue;

	if (Mover)
	{
		Mover->SetShouldMove(IsTriggered);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Mover component is not set in %s"), *GetOwner()->GetActorNameOrLabel());
	}
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivactor"))
	{
		OverlappingActors++;
		
		if (!IsTriggered)
		{
			Trigger(true);
		}
	}
}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivactor"))
	{
		OverlappingActors--;
		
		if (IsTriggered && (OverlappingActors == 0))
		{
			Trigger(false);
		}
	}
}
