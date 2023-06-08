// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
#include "Json.h"
#include "Net/UnrealNetwork.h"
#include "JsonUtilities.h"
#include "HttpModule.h"
#include "UObject/ConstructorHelpers.h"
#include "Interfaces/IHttpResponse.h"

#include "Dalmuti101GameModeBase.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnLoginResponse, bool, Success, const FString, Token);


/**
 * 
 */
UCLASS()
class DALMUTI_API ADalmuti101GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:


	//virtual void LoginRequest() override;

private:
	UFUNCTION(BlueprintCallable, Category = "MyCategory")
	int Foo(int n);		// 테스트용. 추후 삭제


	UFUNCTION(BlueprintCallable, Category = "MyCategory")
	void SignUpRequest(const FString& Username, const FString& Password);
	UFUNCTION(BlueprintCallable, Category = "MyCategory")
	void LoginRequest(const FString& Username, const FString& Password, const FOnLoginResponse& OnLoginResponse);
	void OnCreateGameResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	UFUNCTION(BlueprintCallable, Category = "MyCategory")
	void SendCreateGameRequest();
};
