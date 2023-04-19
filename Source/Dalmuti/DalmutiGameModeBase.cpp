// Copyright Epic Games, Inc. All Rights Reserved.


#include "DalmutiGameModeBase.h"

using namespace std;

void ADalmutiGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// InitialDeck 초기화
	InitialDeck.Init(0, 80);

	// 스폰시킬 BP_PlayerActor 지정
	UBlueprintGeneratedClass* LoadBP = LoadObject<UBlueprintGeneratedClass>(GetWorld(), TEXT("/Game/BP_PlayerActor.BP_PlayerActor_C")); // 경로 끝에 _C

	// InitialDeck에 80장의 카드 번호 입력
	for (int i = 0, card = 1, count = 0; i < 78; i++)
	{
		InitialDeck[i] = card;
		count++;
		if (count == card)
		{
			card++;
			count = 0;
		}
	}
	InitialDeck[78] = 13;
	InitialDeck[79] = 13;

	// playerCount만큼 플레이어 스폰
	for (int i = 0; i < player_count; i++)
	{
		APlayerActor* Player = GetWorld()->SpawnActor<APlayerActor>(LoadBP, FVector(0, i * 500, 0), FRotator::ZeroRotator);
		Players.Add(Player);
		Player->player_index = i;
	}

	// 콘솔에 InitialDeck 출력
	for (int i = 0; i < 80; i++)
		UE_LOG(LogTemp, Log, TEXT("%d"), InitialDeck[i]);

	// Seed 설정 후 InitialDeck 셔플
	int32 Seed = FDateTime::Now().ToUnixTimestamp();
	FRandomStream Stream = FRandomStream(Seed);
	ShuffleArray(Stream, InitialDeck);

	// 모든 플레이어에게 카드 분배
	Distribute();
}

// 배열 셔플
template<typename Type>
static void ADalmutiGameModeBase::ShuffleArray(FRandomStream& Stream, TArray<Type>& Array) {
	const int32 LastIndex = Array.Num() - 1;

	for (int32 i = 0; i <= LastIndex; i += 1) {
		const int32 Index = Stream.RandRange(i, LastIndex);
		if (i == Index) {
			continue;
		}

		Array.Swap(i, Index);
	}
}

// 모든 플레이어에게 카드 분배
void ADalmutiGameModeBase::Distribute()
{
	int count = 0;
	int portion = 80 / player_count;
	
	for (int i = 0; i < player_count; i++)
	{
		UE_LOG(LogTemp, Log, TEXT("distribute"));
		for (int j = 0; j < portion; j++, count++)
		{
			Players[i]->Hand[InitialDeck[count] - 1]++;
		}
	}
	
	for (int i = 0; count < 80 && i < player_count; i++)
	{
		Players[i]->Hand[InitialDeck[count] - 1]++;
	}

	// 콘솔에 분배된 카드 정보 출력
	for (int i = 0; i < player_count; i++) {
		UE_LOG(LogTemp, Log, TEXT("p%d"), i);

		for (int j = 0; j < 13; j++)
			UE_LOG(LogTemp, Log, TEXT("%d"), Players[i]->Hand[j]);
	}
	

}

void ADalmutiGameModeBase::RotateCardNumber(bool b_up)
{
	if (b_up)
	{
		selected_card_number++;
		if (selected_card_number > 12)
			selected_card_number = 1;
	}

	else
	{
		selected_card_number--;
		if (selected_card_number < 1)
			selected_card_number = 12;
	}
}
void ADalmutiGameModeBase::RotateCardCount(bool b_up)
{
	if (b_up)
	{
		selected_card_count++;
		if (selected_card_count > 12)
			selected_card_count = 0;
	}
	
	else
	{
		selected_card_count--;
		if (selected_card_count < 0)
			selected_card_count = 12;
	}
}
void ADalmutiGameModeBase::RotateJokerCount(bool b_up)
{
	if (b_up)
	{
		selected_joker_count++;
		if (selected_joker_count > 2)
			selected_joker_count = 0;
	}

	else
	{
		selected_joker_count--;
		if (selected_joker_count < 0)
			selected_joker_count = 2;
	}
}
void ADalmutiGameModeBase::PlayTurn()
{
	if (!IsValidHand(current_player_index, selected_card_number - 1))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayTurn IndexError"));
		return;
	}

	// 자신이 새롭게 턴을 시작하는 경우.
	if (last_card_number == 0 && last_card_count == 0)
	{
		// EditHand 성공 시
		if (Players[current_player_index]->EditHand(selected_card_number, selected_card_count, selected_joker_count))
		{
			// joker만 단독 사용시 처리
			if (selected_card_count == 0)
			{
				last_card_number = 13;
			}

			else
			{
				last_card_number = selected_card_number;
			}

			last_card_count = selected_card_count + selected_joker_count;
			ResetSkipInfoAll();
			EndTurn();
		}

		// EditHand 실패 시
		else
		{
			UE_LOG(LogTemp, Log, TEXT("PlayTurn - Invalid cardnum, cardcount"));
		}
	}

	// 다른 사람이 낸 카드에 맞춰 내는 경우.
	else
	{
		// 룰 & 조건 체크. if문 안에서 EditHand 실행.
		if (selected_card_number < last_card_number		// 이전 카드보다 낮은 숫자인지?
			&& selected_card_count + selected_joker_count == last_card_count	// 이전 카드와 같은 장수인지?
			&& selected_card_count > 0		// 이전에 카드를 낸 사람이 있으면, 조커만 내는 건 불가능
			&& Players[current_player_index]->EditHand
			(selected_card_number, selected_card_count, selected_joker_count))	// EditHand에 성공했는지?
		{
			last_card_number = selected_card_number;
			ResetSkipInfoAll();
			EndTurn();
		}

		else
		{
			UE_LOG(LogTemp, Log, TEXT("PlayTurn fail."));
		}
	}

	return;
}

void ADalmutiGameModeBase::EndTurn()
{
	current_player_index++;
	if (current_player_index >= player_count)
		current_player_index = 0;
	selected_card_count = last_card_count;
	selected_joker_count = 0;
}

void ADalmutiGameModeBase::SkipTurn()
{
	Players[current_player_index]->bSkiped = true;
	skiped_player_count++;
	// 모든 플레이어가 스킵 시 마지막 카드 정보 리셋.
	if (skiped_player_count == player_count - 1)
	{
		last_card_count = 0;
		last_card_number = 0;
		ResetSkipInfoAll();
	}
	EndTurn();
}

void ADalmutiGameModeBase::ResetSkipInfoAll()
{
	for (int i = 0; i < player_count; i++)
	{
		if (Players.IsValidIndex(i))
			Players[i]->bSkiped = false;
		else
			UE_LOG(LogTemp, Warning, TEXT("ResetSkipInfoAll Index Error"));
	}
	skiped_player_count = 0;
}

bool ADalmutiGameModeBase::IsValidHand(int players_index, int hand_index)
{
	if (Players.IsValidIndex(players_index))
	{
		if (Players[players_index]->Hand.IsValidIndex(hand_index))
			return true;
	}

	return false;
}

// Called every frame
void ADalmutiGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}