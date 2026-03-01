// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	// ...

	StartLocation = GetOwner()->GetActorLocation();
	TargetLocation = StartLocation;
	bShouldMove = false;
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update target based on current state
	TargetLocation = bShouldMove ? (StartLocation + MoveOffset) : StartLocation;

	FVector CurrentLocation = GetOwner()->GetActorLocation();
	bReachedTarget = CurrentLocation.Equals(TargetLocation, 0.1f); // Add tolerance

	if (!bReachedTarget)
	{
		float Speed = MoveOffset.Length() / MoveTime;
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
		GetOwner()->SetActorLocation(NewLocation);
	}
}

void UMover::SetShouldMove(bool NewShouldMove) 
{
	bShouldMove = NewShouldMove;
}

