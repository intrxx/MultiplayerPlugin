// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MSMenuWidgetClass.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"

void UMSMenuWidgetClass::MenuSetup(int32 NumOfPublicConnections, FString TypeOfMatch)
{
	NumPublicConnections = NumOfPublicConnections;
	MatchType = TypeOfMatch;
	
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);
	
	if(UWorld* World = GetWorld())
	{
		if(APlayerController* PC = World->GetFirstPlayerController())
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			
			PC->SetInputMode(InputModeData);

			if(!bIsCursorEnabledInGame)
			{
				PC->SetShowMouseCursor(true);
			}
		}
	}
	
	if(UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if(MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerSystemOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionsSubsystem->MultiplayerSystemOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerSystemOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);

		MultiplayerSessionsSubsystem->MultiplayerSystemOnFindSessionComplete.AddUObject(this, &ThisClass::OnFindSession);
		MultiplayerSessionsSubsystem->MultiplayerSystemOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
	}
}

bool UMSMenuWidgetClass::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	if(HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}

	if(JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}
	
	return true;
}

void UMSMenuWidgetClass::NativeDestruct()
{
	MenuTearDown();
	
	Super::NativeDestruct();
}

void UMSMenuWidgetClass::OnCreateSession(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
			FString("Session Created Successfully"));

		UWorld* World = GetWorld();
		if(World)
		{
			World->ServerTravel("/Game/TopDown/Maps/Lobby?listen");
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
			FString("Failed to create a Session"));
	}
}

void UMSMenuWidgetClass::OnDestroySession(bool bWasSuccessful)
{
}

void UMSMenuWidgetClass::OnStartSession(bool bWasSuccessful)
{
}

void UMSMenuWidgetClass::OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	for (auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
	}
}

void UMSMenuWidgetClass::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
}

void UMSMenuWidgetClass::HostButtonClicked()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
			FString("Host Button Clicked"));
	}

	if(MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

void UMSMenuWidgetClass::JoinButtonClicked()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
			FString("Join Button Clicked"));
	}

	if(MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(20000);
	}
}

void UMSMenuWidgetClass::MenuTearDown()
{
	RemoveFromParent();
	
	if(UWorld* World = GetWorld())
	{
		if(APlayerController* PC  = World->GetFirstPlayerController())
		{
			FInputModeGameOnly InputModeData;
			PC->SetInputMode(InputModeData);

			PC->SetShowMouseCursor(bIsCursorEnabledInGame);
		}
	}
}
