// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.h"
#include "Card.h"
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
	UPROPERTY(EditAnywhere)
		int32 card_offset = 50;	// 카드 간의 거리
	std::list<ACard*> CardList;
	//Side side = Side::bottom;

	UPROPERTY()
		class UPlayerWidget* PlayerWidget;	// 미사용 중.
	UFUNCTION(BlueprintCallable)
	bool EditHand(int card_num, int card_count, int joker_count = 0);

	void SpawnCard();
	void FlipSelectedCards(int card_num, int card_count, int joker_count = 0);		// EditHand 내에서 상대방 플레이어만 호출. 오른쪽 끝의 카드를 뒤집음
	void RemoveSelectedCards();
	void SetPriorityAll();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdatePlayerWidget();	//미사용중
	void TurnOnPlayerWidget();	//미사용중
	void SetCardOffsetAll();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

/*
enum Side
{
	bottom = 0,
	right = 1,
	top = 2,
	left = 3
};
*/