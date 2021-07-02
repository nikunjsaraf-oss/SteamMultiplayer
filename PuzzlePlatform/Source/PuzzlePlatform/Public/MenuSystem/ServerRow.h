// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	UPROPERTY()
	class UMainMenu* Parent;
	
	uint32 Index;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	void Setup(UMainMenu* Parent, uint32 Index);

	UFUNCTION()
	void OnClickHandler();
};
