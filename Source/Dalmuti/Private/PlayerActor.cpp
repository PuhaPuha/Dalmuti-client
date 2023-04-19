// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActor.h"

// Sets default values
APlayerActor::APlayerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Hand.Init(0, 13);
	UE_LOG(LogTemp, Log, TEXT("IM HERE"));
	
}

// Called when the game starts or when spawned
void APlayerActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// card_num에 해당하는 숫자의 카드를 card_count만큼 감소시킴. 실패 시 false 반환
bool APlayerActor::EditHand(int card_num, int card_count, int joker_count)
{
	if (!Hand.IsValidIndex(card_num - 1))
	{
		UE_LOG(LogTemp, Log, TEXT("EditHand-Index Error"));
		return false;
	}

	else if (card_count + joker_count <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("EditHand-No change. count is 0"));
		return false;
	}

	else if (Hand[card_num - 1] - card_count < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("EditHand-CardCount Error"));
		return false;
	}

	else if (Hand[12] - joker_count < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("EditHand-JokerCount Error"));
		return false;
	}
	else
	{
		Hand[card_num - 1] -= card_count;
		Hand[12] -= joker_count;
		UE_LOG(LogTemp, Log, TEXT("EditHand-Complete"));
	}
	return true;
}

// 미사용중. 추후 수정하여 컨트롤러UI에 사용
void APlayerActor::UpdatePlayerWidget()
{
	if (PlayerWidget != nullptr)
	{
		for (int i = 1; i <= 13; i++)
		{
			if (PlayerWidget->GetHandText(i) && Hand.IsValidIndex(i - 1))
			{
				PlayerWidget->GetHandText(i)->SetText(FText::AsNumber(Hand[i - 1]));
			}
		}
	}
}

// 미사용중. 추후 수정하여 컨트롤러UI에 사용
void APlayerActor::TurnOnPlayerWidget()
{
	if (WidgetClass != nullptr)
	{
		PlayerWidget = Cast<UPlayerWidget>(CreateWidget(GetWorld(), WidgetClass));
		if (PlayerWidget != nullptr)
		{
			UpdatePlayerWidget();
			PlayerWidget->AddToViewport();
		}
	}
}
