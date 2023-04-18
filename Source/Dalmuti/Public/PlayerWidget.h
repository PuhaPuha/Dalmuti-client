// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class DALMUTI_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* one;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* two;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* three;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* four;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* five;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* six;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* seven;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* eight;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* nine;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ten;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* eleven;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* twelve;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* joker;
public:
	UTextBlock* GetHandText(int num);
};
