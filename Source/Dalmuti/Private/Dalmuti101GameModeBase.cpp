// Fill out your copyright notice in the Description page of Project Settings.

#include "Dalmuti101GameModeBase.h"

// 테스트용. 추후 삭제
int ADalmuti101GameModeBase::Foo(int n)
{
	UE_LOG(LogTemp, Warning, TEXT("Get Base SUCCESS %d"), n);
	
	return n;
}

void ADalmuti101GameModeBase::SignUpRequest(const FString& Username, const FString& Password)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("username"), Username);
	JsonObject->SetStringField(TEXT("password"), Password);

	FString JsonString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetURL(TEXT("http://54.180.104.184:8080/api/signup"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(JsonString);
	HttpRequest->ProcessRequest();
}

void ADalmuti101GameModeBase::LoginRequest(const FString& Username, const FString& Password, const FOnLoginResponse& OnLoginResponse)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField(TEXT("username"), Username);
    JsonObject->SetStringField(TEXT("password"), Password);

    FString JsonString;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetURL(TEXT("http://54.180.104.184:8080/api/authenticate"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetContentAsString(JsonString);

    HttpRequest->OnProcessRequestComplete().BindLambda([OnLoginResponse](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        if (bWasSuccessful && Response.IsValid())
        {
            int32 ResponseCode = Response->GetResponseCode();
            if (ResponseCode == 200)
            {
                FString ResponseContent = Response->GetContentAsString();
                TSharedPtr<FJsonObject> JsonObject;
                TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseContent);
                if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
                {
                    if (JsonObject->HasField(TEXT("token")))
                    {
                        FString Token = JsonObject->GetStringField(TEXT("token"));
                        OnLoginResponse.ExecuteIfBound(true, Token);
                        return;
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Login failed: Token not found in the response."));
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to parse login response JSON."));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Login request failed with response code: %d"), ResponseCode);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Login request failed."));
        }

        OnLoginResponse.ExecuteIfBound(false, TEXT(""));
    });

    HttpRequest->ProcessRequest();
}

void DALMUTI_API::ADalmuti101GameModeBase::OnCreateGameResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully && Response.IsValid())
	{
		int32 ResponseCode = Response->GetResponseCode();
		if (EHttpResponseCodes::IsOk(ResponseCode))
		{
			FString ResponseData = Response->GetContentAsString();
			
			UE_LOG(LogTemp, Warning, TEXT("Create Game Response Received: %s"), *ResponseData);
		}
		else
		{
			FString ErrorMsg = FString::Printf(TEXT("Create Game Request failed with response code: %d"), ResponseCode);
		
			UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMsg);
		}
	}
	else
	{
		FString ErrorMsg = TEXT("Create Game Connection Error");
	
		UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMsg);
	}
}

void DALMUTI_API::ADalmuti101GameModeBase::SendCreateGameRequest()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	FString Url = " http://54.180.104.184:8080/room";
	Request->SetURL(Url);

	Request->SetVerb("POST");

	Request->SetContentAsString(TEXT("Room name"));

	Request->OnProcessRequestComplete().BindUObject(this, &ADalmuti101GameModeBase::OnCreateGameResponseReceived);

	Request->ProcessRequest();
}
