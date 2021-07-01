// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatform/Public/PuzzlePlatformGameInstance.h"

#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "PuzzlePlatform/Public/MenuSystem/MenuWidget.h"
#include "UObject/ConstructorHelpers.h"

static const FName GSession_Name = TEXT("Session - 1");

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> PauseBPClass(TEXT("/Game/MenuSystem/WBP_PauseMenu"));
	if (!ensure(PauseBPClass.Class != nullptr)) return;

	PauseMenuClass = PauseBPClass.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
				this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
				this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
				this, &UPuzzlePlatformGameInstance::OnFindSessionsComplete);

			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid())
			{
				// SessionSearch->bIsLanQuery = true;
				UE_LOG(LogTemp, Warning, TEXT("Searching Sessions"));
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no Subsystem"));
	}
}

void UPuzzlePlatformGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;

	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();

	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::LoadPauseMenu()
{
	if (!ensure(PauseMenuClass != nullptr)) return;

	UMenuWidget* PauseGame = CreateWidget<UMenuWidget>(this, PauseMenuClass);
	if (!ensure(PauseGame != nullptr)) return;

	PauseGame->Setup();

	PauseGame->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create Session"));
		return;
	}
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccess) const
{
	if (bSuccess)
	{
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::CreateSession() const
{
	if (!SessionInterface.IsValid()) return;
	FOnlineSessionSettings OnlineSessionSettings;
	
	OnlineSessionSettings.bIsLANMatch = true;
	OnlineSessionSettings.NumPublicConnections = 2;
	OnlineSessionSettings.bShouldAdvertise = true; 
	
	SessionInterface->CreateSession(0, GSession_Name, OnlineSessionSettings);
}

void UPuzzlePlatformGameInstance::OnFindSessionsComplete(bool bSuccess)
{
	if(bSuccess && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Search finish"));
		for(const FOnlineSessionSearchResult Result :  SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session: %s"), *Result.GetSessionIdStr());
		}
	}
}


void UPuzzlePlatformGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(GSession_Name);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(GSession_Name);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 10, FColor::Green, TEXT("Leaving...."));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	if (PlayerController->GetCharacter()->HasAuthority())
	{
		UWorld* World = GetWorld();
		if (!ensure(World)) { return; }
		World->ServerTravel("/Game/MenuSystem/MainMenu");
		return;
	}
	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", TRAVEL_Absolute);
}
