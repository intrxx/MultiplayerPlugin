// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSMenuWidgetClass.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMSMenuWidgetClass : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup();
};
