// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/PauseGame.h"

#include "Components/Button.h"

bool UPauseGame::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UPauseGame::CloseMenu);

	if (!ensure(LeaveButton != nullptr)) return false;
	LeaveButton->OnClicked.AddDynamic(this, &UPauseGame::LeaveGame);

	return true;
}

void UPauseGame::CloseMenu()
{
	Teardown();
}

void UPauseGame::LeaveGame()
{
	if (MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadMainMenu();
	}
}
