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
		// Check the response code to handle success or failure cases
		int32 ResponseCode = Response->GetResponseCode();
		if (EHttpResponseCodes::IsOk(ResponseCode))
		{
			// The request was successful, process the response data
			FString ResponseData = Response->GetContentAsString();
			// Process the response data as needed

			// Print the response data to the output log
			UE_LOG(LogTemp, Warning, TEXT("Create Game Response Received: %s"), *ResponseData);
		}
		else
		{
			// The request failed, handle the error
			FString ErrorMsg = FString::Printf(TEXT("Create Game Request failed with response code: %d"), ResponseCode);
			// Handle the error message as needed

			// Print the error message to the output log
			UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMsg);
		}
	}
	else
	{
		// The request was not successfully connected, handle the connection error
		FString ErrorMsg = TEXT("Create Game Connection Error");
		// Handle the error message as needed

		// Print the error message to the output log
		UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMsg);
	}
}

void DALMUTI_API::ADalmuti101GameModeBase::SendCreateGameRequest()
{
	// Create an HTTP request object
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	// Set the request URL
	FString Url = " http://54.180.104.184:8080/room";
	Request->SetURL(Url);

	// Set the request verb to POST
	Request->SetVerb("POST");

	// Set the request headers, if needed
	// Request->SetHeader(TEXT("HeaderName"), TEXT("HeaderValue"));

	// Set the request content, if needed
	Request->SetContentAsString(TEXT("Room name"));

	// Set the callback function to handle the response
	Request->OnProcessRequestComplete().BindUObject(this, &ADalmuti101GameModeBase::OnCreateGameResponseReceived);

	// Send the request
	Request->ProcessRequest();
}
