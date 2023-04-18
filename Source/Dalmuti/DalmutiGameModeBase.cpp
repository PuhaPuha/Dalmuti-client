// Copyright Epic Games, Inc. All Rights Reserved.


#include "DalmutiGameModeBase.h"

using namespace std;

void ADalmutiGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	initialDeck.Init(0, 80);

	UBlueprintGeneratedClass* LoadBP = LoadObject<UBlueprintGeneratedClass>(GetWorld(), TEXT("/Game/BP_PlayerActor.BP_PlayerActor_C")); // 경로 끝에 _C

	for (int i = 0, card = 1, count = 0; i < 78; i++)
	{
		initialDeck[i] = card;
		count++;
		if (count == card)
		{
			card++;
			count = 0;
		}
	}
	initialDeck[78] = 13;
	initialDeck[79] = 13;
	for (int i = 0; i < playerCount; i++)
	{
		APlayerActor* Player = GetWorld()->SpawnActor<APlayerActor>(LoadBP, FVector(0, i * 500, 0), FRotator::ZeroRotator);
		Players.Add(Player);
	}
	for (int i = 0; i < 80; i++)
		UE_LOG(LogTemp, Log, TEXT("%d"), initialDeck[i]);
	int32 Seed = FDateTime::Now().ToUnixTimestamp();
	FRandomStream Stream = FRandomStream(Seed);
	ShuffleArray(Stream, initialDeck);
	Distribute();
}

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


void ADalmutiGameModeBase::Distribute()
{
	int count = 0;
	int portion = 80 / playerCount;
	
	for (int i = 0; i < playerCount; i++)
	{
		UE_LOG(LogTemp, Log, TEXT("distribute"));
		for (int j = 0; j < portion; j++, count++)
		{
			Players[i]->Hand[initialDeck[count] - 1]++;
		}
	}
	
	for (int i = 0; count < 80 && i < playerCount; i++)
	{
		Players[i]->Hand[initialDeck[count] - 1]++;
	}

	for (int i = 0; i < playerCount; i++) {
		UE_LOG(LogTemp, Log, TEXT("p%d"), i);

		for (int j = 0; j < 13; j++)
			UE_LOG(LogTemp, Log, TEXT("%d"), Players[i]->Hand[j]);
	}
	

}
