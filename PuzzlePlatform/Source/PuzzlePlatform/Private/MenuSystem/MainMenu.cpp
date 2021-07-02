// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatform/Public/MenuSystem/MainMenu.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MenuSystem/ServerRow.h"
#include "UObject/ConstructorHelpers.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(BackButton != nullptr)) return false;
	BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinGame != nullptr)) return false;
	JoinGame->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);


	return true;
}


void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	UWorld* World = this->GetWorld();
	if(!World) return;

	ServerList->ClearChildren();

	// To store number of servers present in the game.
	uint32 i = 0;
	
	for(const FString& ServerName : ServerNames)
	{
		UServerRow* ServerRow = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(ServerRow != nullptr)) return;
		ServerRow->ServerName->SetText(FText::FromString(ServerName));
		ServerRow->Setup(this, i);
		++i;
		
		ServerList->AddChild(ServerRow);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
}


void UMainMenu::JoinServer()
{
	if(SelectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index is: %d"), SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set"));
	}
	if (MenuInterface != nullptr)
	{
		// if (!ensure(IPAddressField != nullptr)) return;
		// const FString Address = IPAddressField->GetText().ToString();
		MenuInterface->Join("");
	}
}




void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if(MenuInterface)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::QuitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, false);
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}
