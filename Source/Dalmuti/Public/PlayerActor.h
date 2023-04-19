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
		int32 player_index;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info)
		TArray<int32> Hand;		// 손패를 나타내는 크기 13의 배열. 인덱스는 카드번호, 값은 개수를 의미함.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Info)
		bool bSkiped = false;

	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UUserWidget> WidgetClass;	// 미사용 중.
	UPROPERTY()
		class UPlayerWidget* PlayerWidget;	// 미사용 중.
	UFUNCTION(BlueprintCallable)
	bool EditHand(int card_num, int card_count, int joker_count = 0);

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
