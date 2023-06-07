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

// AI 낼카드 정하기. gamemode가 호출함. 낼 카드가 없으면 false 리턴
bool APlayerActor::SelectCardAI(int* selected_card_number, int* selected_card_count, int* selected_joker_count, 
	int last_card_number, int last_card_count)
{
	int type_count = 0;

	if (Hand.Num() < 13)
	{
		// Hand 배열의 크기가 충분하지 않을 때 예외 처리
		UE_LOG(LogTemp, Error, TEXT("Hand 배열의 크기가 충분하지 않습니다."));
		return false;
	}

	for (int32 i = 0; i < Hand.Num(); i++)
	{
		if (Hand[i] > 0)
		{
			type_count++;
		}
	}

	// 턴새로시작
	if (last_card_number == 0 && last_card_count == 0)
	{
		// 들고있는 카드종류 3장이하일때
		if (type_count <= 3)
		{
			for (int i = 0; i < Hand.Num(); i++)
			{
				if (Hand.IsValidIndex(i) && Hand[i] > 0)
				{
					*selected_card_number = i + 1;
					*selected_card_count = Hand[i];
					*selected_joker_count = Hand[12];
					return true;
				}
			}
		}

		// 4장 이상일때. 가장 높은숫자부터 냄
		else
		{
			for (int i = Hand.Num() - 2; i >= 0; i--)
			{
				if (Hand.IsValidIndex(i) && Hand[i] > 0)
				{
					*selected_card_number = i + 1;
					*selected_card_count = Hand[i];
					*selected_joker_count = 0;
					return true;
				}
			}
		}
	}

	// 턴 이어받는상황
	else
	{
		for (int i = last_card_number - 2; i >= 0; i--)
		{
			if (Hand.IsValidIndex(i) && Hand[i] == last_card_count)
			{
				*selected_card_number = i + 1;
				*selected_card_count = Hand[i];
				*selected_joker_count = 0;
				return true;
			}
		}

		// 2~4사이의 카드면 조커 포함해서 내기
		if (Hand.IsValidIndex(12) && Hand[12] > 0 && last_card_count > 1 && last_card_count < 5)
		{
			for (int i = last_card_number - 2; i >= 0; i--)
			{
				if (Hand.IsValidIndex(i) && Hand[i] + 1 == last_card_count)
				{
					*selected_card_number = i + 1;
					*selected_card_count = Hand[i];
					*selected_joker_count = 1;
					return true;
				}
			}
		}
	}

	return false;
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
		if (player_index != 0)
		{
			FlipSelectedCards(card_num, card_count, joker_count);
		}
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

void APlayerActor::FlipSelectedCards(int card_num, int card_count, int joker_count)
{
	for (auto CardIt = CardList.rbegin(); CardIt != CardList.rend(); ++CardIt)
	{
		ACard* Card = *CardIt;
		if (joker_count > 0)
		{
			Card->bFliping = true;
			Card->number = 13;
			Card->bSelected = true;
			joker_count--;
			Card->HeadingDirection.Yaw = 0.f;
		}
		else if (card_count > 0)
		{
			Card->bFliping = true;
			Card->number = card_num;
			Card->bSelected = true;
			card_count--;
			Card->HeadingDirection.Yaw = 0.f;
		}
		else
		{
			break;
		}
	}
}

void APlayerActor::RemoveSelectedCards()
{
	auto it = CardList.begin();
	while (it != CardList.end())
	{
		if ((*it)->bSelected)
		{
			(*it)->bSelected = false;
			it = CardList.erase(it);
		}
		else
		{
			++it;
		}
	}
	SetCardOffsetAll();
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

void APlayerActor::SetPriorityAll()
{
	int count = 0;
	for (ACard* cd : CardList)
	{
		cd->BackSprite->SetTranslucentSortPriority(count);
		cd->FrontSprite->SetTranslucentSortPriority(count);
		count++;
	}
}

void APlayerActor::SetCardOffsetAll()
{
	int i = 0;
	for (ACard* Card : CardList)
	{
		FVector Forward = GetActorForwardVector();
		Forward.Normalize();
		FVector RightDirection = Forward.RotateAngleAxis(90.0f, FVector::UpVector);
		Card->Destination = GetActorLocation() + RightDirection * i * card_offset + FVector(0, 0, i * 1);
		//Card->offset_y = i * card_offset;
		SetPriorityAll();
		i++; 
		//UE_LOG(LogTemp, Log, TEXT("%f"), Card->offset_y);
	}
}

void APlayerActor::SpawnCard()
{
	// 스폰시킬 BP_Card 지정
	UBlueprintGeneratedClass* LoadBP = LoadObject<UBlueprintGeneratedClass>(GetWorld(), TEXT("/Game/Gameplay/BP_Card.BP_Card_C")); // 경로 끝에 _C


	FVector CardLocation = this->GetActorLocation();

	// Hand의 정보대로 BP_Card 스폰
	for (int i = 0; i < Hand.Num(); i++)
	{
		for (int j = 0; j < Hand[i]; j++)
		{
			ACard* Card = GetWorld()->SpawnActor<ACard>(LoadBP, CardLocation, this->GetActorRotation());
			Card->HeadingDirection = FRotator(0, this->GetActorRotation().Yaw, 0);
			if (player_index == 0)
			{
				Card->number = i + 1;
				//Card->bControllerble = true;
				//Card->SetFront(Hand[i]);
				//Card->Flip();
			}

			else
			{
				Card->number = 0;
			}


			CardList.push_back(Card);
		}
	}
	SetCardOffsetAll();
}
