// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MSMenuWidgetClass.h"

void UMSMenuWidgetClass::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PC = World->GetFirstPlayerController();

		if(PC)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			
			PC->SetInputMode(InputModeData);

			PC->SetShowMouseCursor(true);
		}
	}
}
