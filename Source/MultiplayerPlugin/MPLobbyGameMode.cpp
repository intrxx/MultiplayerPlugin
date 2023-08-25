// Fill out your copyright notice in the Description page of Project Settings.


#include "MPLobbyGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void AMPLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(GameState)
	{
		int NumberOfPlayers = GameState.Get()->PlayerArray.Num();

		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Orange,
				FString::Printf(TEXT("Players in game: %d"), NumberOfPlayers));

			APlayerState* PS = NewPlayer->GetPlayerState<APlayerState>();

			if(PS)
			{
				FString PlayerName = PS->GetPlayerName();
				
				GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan,
				FString::Printf(TEXT("%s has joined the game."), *PlayerName));
			}
		}
	}
}

void AMPLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if(GameState)
	{
		int NumberOfPlayers = GameState.Get()->PlayerArray.Num();

		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Orange,
				FString::Printf(TEXT("Player's in game: %d"), NumberOfPlayers - 1));

			APlayerState* PS = Exiting->GetPlayerState<APlayerState>();

			if(PS)
			{
				FString PlayerName = PS->GetPlayerName();
				
				GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan,
				FString::Printf(TEXT("%s has exited the game."), *PlayerName));
			}
		}
	}
}
