// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

	FVector GlobalStartPosition;
	FVector GlobalTargetPosition;

	UPROPERTY(EditAnywhere, Category="Properties")
	float Speed = 5;

	UPROPERTY(EditAnywhere, Category="Properties", meta = (MakeEditWidget = true))
	FVector TargetPosition;

	UPROPERTY(EditAnywhere, Category="Platform")
	int32 ActivePlatform = 1;

public:
	AMovingPlatform();

	virtual void Tick(float DeltaTime) override;

	void AddActiveTrigger();
	void RemoveActiveTrigger();

protected:
	virtual void BeginPlay() override;
};
