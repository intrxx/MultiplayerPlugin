// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MSMenuWidgetClass.h"
#include "MultiplayerSessionsSubsystem.h"
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

			PC->SetShowMouseCursor(true);
		}
	}
	
	if(UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
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

		UWorld* World = GetWorld();
		if(World)
		{
			World->ServerTravel("/Game/TopDown/Maps/Lobby?listen");
		}
	}
}

void UMSMenuWidgetClass::JoinButtonClicked()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
			FString("Join Button Clicked"));
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
