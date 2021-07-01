// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MenuWidget.h"

#include "PauseGame.generated.h"

UCLASS()
class PUZZLEPLATFORM_API UPauseGame : public UMenuWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* LeaveButton;

	UFUNCTION()
	void CloseMenu();
	
	UFUNCTION()
	void LeaveGame();
};
