// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerActor.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "DalmutiGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DALMUTI_API ADalmutiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		int32 playerCount = 4;
	UPROPERTY(EditAnywhere)
		TArray<APlayerActor*> Players;
	UPROPERTY(EditDefaultsOnly, Category = "Player")
		TSubclassOf<class APlayerActor> PlayerActorClass;
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
	void Distribute();
	TArray<int32> initialDeck;
	template<typename Type>
	static void ShuffleArray(FRandomStream& Stream, TArray<Type>& Array);

};

