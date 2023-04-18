// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.h"
#include "PlayerActor.generated.h"

UCLASS()
class DALMUTI_API APlayerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info)
		TArray<int32> Hand;
	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UUserWidget> WidgetClass;
	UPROPERTY()
		class UPlayerWidget* PlayerWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SwitchWidget();
	void UpdatePlayerWidget();
	void TurnOnPlayerWidget();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
