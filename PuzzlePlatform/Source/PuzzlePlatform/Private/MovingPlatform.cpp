// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// To replicate only on Server. In client we don't have to run this.
	if (HasAuthority())
	{
		// Set whether this actor replicates to network clients.
		SetReplicates(true);

		// Set whether this actor's movement replicates to network clients.
		SetReplicateMovement(true);
	}

	GlobalStartPosition = GetActorLocation();
	GlobalTargetPosition = GetTransform().TransformPosition(TargetPosition);
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActivePlatform > 0)
	{
		// Will only run on Server
		if (HasAuthority())
		{
			FVector ActorLocation = GetActorLocation();
			const float JourneyLength = (GlobalTargetPosition - GlobalStartPosition).Size();
			const float JourneyTravelled = (ActorLocation - GlobalStartPosition).Size();

			if (JourneyTravelled >= JourneyLength)
			{
				const FVector Swap = GlobalStartPosition;
				GlobalStartPosition = GlobalTargetPosition;
				GlobalTargetPosition = Swap;
			}

			const FVector Direction = (GlobalTargetPosition - GlobalStartPosition).GetSafeNormal();
			ActorLocation += Speed * DeltaTime * Direction;
			SetActorLocation(ActorLocation);
		}
	}
}

void AMovingPlatform::AddActiveTrigger()
{
	ActivePlatform++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if (ActivePlatform > 0)
	{
		ActivePlatform--;
	}
}
