// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerActor.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/EngineTypes.h"
#include "DalmutiGameModeBase.generated.h"

/* GameModeBase는 룰과 턴 진행, 전체 플레이어를 관리함
 */
UCLASS()
class DALMUTI_API ADalmutiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		int32 player_count = 4;
	UPROPERTY(EditAnywhere)
		TArray<APlayerActor*> Players;	// 플레이어는 스폰된 이후 해당 배열에 추가됨
	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<class APlayerActor> PlayerActorClass;	// 미사용 중.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 last_card_number = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 last_card_count = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 last_player_index = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 current_player_index = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 selected_card_number = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 selected_card_count = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 selected_joker_count = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 skiped_player_count = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector DeckLocation = FVector(0.f, -500.f, 0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector DeckForwardVector = FVector(1.f, 0.f, 0.f);
	std::list<ACard*> CardList;		// 화면 중앙에 펼쳐진 카드의 리스트
	std::list<ACard*> DeckList;		// 화면 중앙 왼쪽에 뭉쳐놓을 카드의 리스트
	UPROPERTY(EditAnywhere)
		int32 card_offset = 50;	// 카드 간의 거리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool game_ended = false;

	/*
	UFUNCTION(BlueprintPure)
		TArray<int32> GetHand(int32 i)
	{
		if (players.IsEmpty())
		{
			players.Init(FPlayerInfo(), playerCount);
		}
		if (i >= playerCount)
			return players[i / playerCount].hand;

		return players[i].hand;
		
	}
	*/
	
protected:
	virtual void BeginPlay() override;

private:
	int win_count = 0;
	void PlaySound();
	TArray<int32> InitialDeck;	// 카드 분배 전 80장의 카드를 담을 배열
	void Distribute();	// 모든 플레이어에게 카드 분배
	void ResetSkipInfoAll();	// 카드를 내는 플레이어가 있으면 호출해서 bSkiped 변수를 전부 false로 초기화.
	void UpdateSkipInfo();
	bool IsValidHand(int players_index, int hand_index);	// 핸드 접근 전 NULL체크
	template<typename Type>
	static void ShuffleArray(FRandomStream& Stream, TArray<Type>& Array);	// 카드 덱 셔플
	TArray<FVector> PlayerSpawnPoint;
	void InitPlayerSpawnPoint();
	void HandToCenter(std::list<ACard*> HandCardList);
	bool IsCurrentPlayerWin();
	void EndGame();
	void Autoplay();

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	int GetPlayerState(int player_num);

	UFUNCTION(BlueprintCallable)
	void RotateCardNumber(bool b_up = true);
	UFUNCTION(BlueprintCallable)
	void RotateCardCount(bool b_up = true);
	UFUNCTION(BlueprintCallable)
	void RotateJokerCount(bool b_up = true);
	UFUNCTION(BlueprintCallable)
	void PlayTurn(bool isUser = false);
	UFUNCTION(BlueprintCallable)
	void EndTurn();
	UFUNCTION(BlueprintCallable)
	void SkipTurn(bool isUser = false);

};

