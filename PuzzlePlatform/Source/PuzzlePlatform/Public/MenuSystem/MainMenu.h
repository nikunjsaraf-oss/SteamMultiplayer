// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MenuWidget.h"

#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FString> ServerNames);

	void SelectIndex(uint32 Index);

protected:

	virtual bool Initialize() override;

private:

	TSubclassOf<UUserWidget> ServerRowClass;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGame;

	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void OpenMainMenu();

	TOptional<uint32> SelectedIndex;

};
