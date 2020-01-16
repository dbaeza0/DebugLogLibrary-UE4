// Copyright Ali El Saleh 2019

#include "Log.h"
#include "Debug.h"

#include "Engine/Engine.h"

#include "Math/Vector.h"

#include "DebugLogLibrarySettings.h"

#include <sstream>

const UDebugLogLibrarySettings* ULog::Settings;

void ULog::PostInitProperties()
{
	Super::PostInitProperties();

	Settings = GetDefault<UDebugLogLibrarySettings>();
}

void ULog::ObjectValidity(UObject* ObjectRef, const ELoggingOptions LoggingOption)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (LoggingOption == LO_Viewport)
	{
		if (ObjectRef)
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, Settings->SuccessColor, NET_MODE_PREFIX + ObjectRef->GetName() + " is valid");
		else
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, Settings->ErrorColor, NET_MODE_PREFIX + FString("Object is null"));
	}
	else if (LoggingOption == LO_Console)
	{
		if (ObjectRef)
			UE_LOG(LogObjectValidity, Warning, TEXT("%s%s is valid"), NET_MODE_PREFIX, *ObjectRef->GetName())
		else
			UE_LOG(LogObjectValidity, Error, TEXT("%sObject is null"), NET_MODE_PREFIX)
	}
	else if (LoggingOption == LO_Both)
	{
		if (ObjectRef)
		{
			UE_LOG(LogObjectValidity, Warning, TEXT("%s%s is valid"), NET_MODE_PREFIX, *ObjectRef->GetName())
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, Settings->SuccessColor, NET_MODE_PREFIX + ObjectRef->GetName() + " is valid");
		}
		else
		{
			UE_LOG(LogObjectValidity, Error, TEXT("%sObject is null"), NET_MODE_PREFIX)	
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, Settings->ErrorColor, NET_MODE_PREFIX + FString("Object is null"));
		}
	}
#endif
}

void ULog::DebugMessage(const EDebugLogType LogSeverity, const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	switch (LogSeverity)
	{
	case DL_Info:
		Info(Message, LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Success:
		Success(Message, LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Warning:
		Warning(Message, LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Error:
		Error(Message, LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Fatal:
		UE_LOG(LogFatal, Fatal, TEXT("%s%s"), NET_MODE_PREFIX, *Message)
	break;

	default:
		break;
	}
}

void ULog::DebugMessage(const EDebugLogType LogSeverity, const FName& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{	
	switch (LogSeverity)
	{
	case DL_Info:
		Info(Message.ToString(), LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Success:
		Success(Message.ToString(), LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Warning:
		Warning(Message.ToString(), LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Error:
		Error(Message.ToString(), LoggingOption, bAddPrefix, TimeToDisplay);
	break;

	case DL_Fatal:
		UE_LOG(LogFatal, Fatal, TEXT("%s"), *Message.ToString())
	break;

	default:
	break;
	}
}

void ULog::DebugMessage_WithCondition(const EDebugLogType LogSeverity, const bool bCondition, const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	if (bCondition)
		DebugMessage(LogSeverity, Message, LoggingOption, bAddPrefix, TimeToDisplay);
}

void ULog::Crash(const FString& Message)
{
	UE_LOG(LogCrash, Fatal, TEXT("%s%s"), NET_MODE_PREFIX, *Message)
}

void ULog::Fatal(const FString& Message)
{
	UE_LOG(LogFatal, Fatal, TEXT("%s%s"), NET_MODE_PREFIX, *Message)
}

void ULog::Fatal_WithCondition(const FString& Message, const bool bCondition)
{
	if (bCondition)
		Fatal(Message);
}

void ULog::Error(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	FString NewMessage;
	if (bAddPrefix && LoggingOption != LO_Console)
		NewMessage = NET_MODE_PREFIX + FString("Error: ") + Message;
	else
		NewMessage = NET_MODE_PREFIX + Message;

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->ErrorColor, NewMessage);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogError, Error, TEXT("%s%s"), NET_MODE_PREFIX, *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogError, Error, TEXT("%s%s"), NET_MODE_PREFIX, *Message)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->ErrorColor, NewMessage);
	}
}

void ULog::Error_WithCondition(const FString& Message, const bool bCondition, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	if (bCondition)
		Error(Message, LoggingOption, bAddPrefix, TimeToDisplay);
}

void ULog::Success(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	FString NewMessage;
	if (bAddPrefix)
		NewMessage = NET_MODE_PREFIX + FString("Success: ") + Message;
	else
		NewMessage = NET_MODE_PREFIX + Message;
	
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->SuccessColor, NewMessage);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%s"), *NewMessage)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%s"), *NewMessage)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->SuccessColor, NewMessage);
	}
}

void ULog::Success_WithCondition(const FString& Message, const bool bCondition, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	if (bCondition)
		Success(Message, LoggingOption, bAddPrefix, TimeToDisplay);
}

void ULog::Warning(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	FString NewMessage;
	if (bAddPrefix && LoggingOption != LO_Console)
		NewMessage = NET_MODE_PREFIX + FString("Warning: ") + Message;
	else
		NewMessage = NET_MODE_PREFIX + Message;
	
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->WarningColor, NewMessage);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogWarning, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogWarning, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Message)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->WarningColor, NewMessage);
	}
}

void ULog::Warning_WithCondition(const FString& Message, const bool bCondition, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	if (bCondition)
		Warning(Message, LoggingOption, bAddPrefix, TimeToDisplay);
}

void ULog::Info(const FString& Message, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	FString NewMessage;
	if (bAddPrefix)
		NewMessage = NET_MODE_PREFIX + FString("Info: ") + Message;
	else
		NewMessage = NET_MODE_PREFIX + Message;
	
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NewMessage);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Display, TEXT("%s"), *NewMessage)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogMessage, Display, TEXT("%s"), *NewMessage)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NewMessage);
	}
}

void ULog::Info_WithCondition(const FString& Message, const bool bCondition, const ELoggingOptions LoggingOption, const bool bAddPrefix, const float TimeToDisplay)
{
	if (bCondition)
		Info(Message, LoggingOption, bAddPrefix, TimeToDisplay);
}

void ULog::Hello(const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->InfoColor, NET_MODE_PREFIX + FString("Hello"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Warning, TEXT("%sHello"), NET_MODE_PREFIX)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogMessage, Warning, TEXT("%sHello"), NET_MODE_PREFIX)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->InfoColor, NET_MODE_PREFIX + FString("Hello"));
	}
}

void ULog::Yes(const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->InfoColor, NET_MODE_PREFIX + FString("Yes"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Warning, TEXT("%sYes"), NET_MODE_PREFIX)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogMessage, Warning, TEXT("%sYes"), NET_MODE_PREFIX)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->InfoColor, NET_MODE_PREFIX + FString("Yes"));
	}
}

void ULog::Yes(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
	const FString Message = NET_MODE_PREFIX + Prefix + "Yes" + Suffix;

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->InfoColor, Message);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMessage, Warning, TEXT("%s"), *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogMessage, Warning, TEXT("%s"), *Message)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->InfoColor, Message);
	}
}

void ULog::No(const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->InfoColor, NET_MODE_PREFIX + FString("No"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogWarning, Warning, TEXT("%sNo"), NET_MODE_PREFIX)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogWarning, Warning, TEXT("%sNo"), NET_MODE_PREFIX)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->InfoColor, NET_MODE_PREFIX + FString("No"));
	}
}

void ULog::Valid(const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->SuccessColor, NET_MODE_PREFIX + FString("Valid"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%sValid"), NET_MODE_PREFIX)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%sValid"), NET_MODE_PREFIX)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->SuccessColor, NET_MODE_PREFIX + FString("Valid"));
	}
}

void ULog::Invalid(const ELoggingOptions LoggingOption)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->ErrorColor, NET_MODE_PREFIX + FString("Invalid"));
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogError, Error, TEXT("%sInvalid"), NET_MODE_PREFIX)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogError, Error, TEXT("%sInvalid"), NET_MODE_PREFIX)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->ErrorColor, NET_MODE_PREFIX + FString("Invalid"));
	}
}

void ULog::No(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
	const FString Message = NET_MODE_PREFIX + Prefix + ": No" + Suffix;

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->InfoColor, Message);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogWarning, Warning, TEXT("%s"), *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogWarning, Warning, TEXT("%s"), *Message)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->InfoColor, Message);
	}
}

void ULog::Valid(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
	const FString Message = NET_MODE_PREFIX + Prefix + FString("Valid") + Suffix;

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->SuccessColor, Message);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->SuccessColor, Message);
	}
}

void ULog::Invalid(const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption)
{
	const FString Message = NET_MODE_PREFIX + Prefix + FString("Invalid") + Suffix;

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->ErrorColor, Message);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogError, Error, TEXT("%s"), *Message)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogError, Error, TEXT("%s"), *Message)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, Settings->ErrorColor, Message);
	}
}

void ULog::Number(const int8 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int16 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int32 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int64 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint8 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint16 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint32 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint64 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const float Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const double Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const long Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogLongInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int8 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int16 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int32 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const int64 Number, EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint8 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint16 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint32 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const uint64 Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogUInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const float Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const double Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number(const long Number, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogLongInt(Number, "", "", NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Percent(const float Number, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	const FString NewMessage = NET_MODE_PREFIX + Prefix + FString::SanitizeFloat(Number) + "%" + Suffix;

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NewMessage);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s"), *NewMessage)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s"), *NewMessage)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NewMessage);
	}
}

void ULog::Percent(const float Number, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Percent(Number, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Bool(const bool bBoolToTest, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		if (bBoolToTest)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + "True" + Suffix);
		else
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + "False" + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		if (bBoolToTest)
			UE_LOG(LogBool, Warning, TEXT("%s%sTrue%s"), NET_MODE_PREFIX, *Prefix, *Suffix)
		else
			UE_LOG(LogBool, Warning, TEXT("%s%sFalse%s"), NET_MODE_PREFIX, *Prefix, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		if (bBoolToTest)
		{
			UE_LOG(LogBool, Warning, TEXT("%s%sTrue%s"), NET_MODE_PREFIX, *Prefix, *Suffix)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + "True" + Suffix);
		}
		else
		{
			UE_LOG(LogBool, Warning, TEXT("%s%sFalse%s"), NET_MODE_PREFIX, *Prefix, *Suffix)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + "False" + Suffix);
		}
	}
}

void ULog::Bool(const bool bBoolToTest, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Bool(bBoolToTest, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Vector(const FVector& InVector, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + InVector.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogVector, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *InVector.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogVector, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *InVector.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + InVector.ToString() + Suffix);
	}
}

void ULog::Vector(const FVector& InVector, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Vector(InVector, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Rotator(const FRotator& InRotator, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + InRotator.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogRotator, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *InRotator.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogRotator, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *InRotator.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + InRotator.ToString() + Suffix);
	}
}

void ULog::Rotator(const FRotator& InRotator, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Rotator(InRotator, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Transform(const FTransform& InTransform, const FString& Prefix, const bool bFormat, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		if (bFormat)
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Scale: ") + InTransform.GetScale3D().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Rotation: ") + InTransform.GetRotation().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + InTransform.GetLocation().ToString());

			if (!Prefix.IsEmpty())
				GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + InTransform.ToString());
		}
	}
	else if (LoggingOption == LO_Console)
	{
		if (bFormat)
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
			UE_LOG(LogTransform, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *InTransform.GetLocation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sRotation: %s"), NET_MODE_PREFIX, *InTransform.GetRotation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sScale: %s"), NET_MODE_PREFIX, *InTransform.GetScale3D().ToString())
		}
		else
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s%s"), NET_MODE_PREFIX, *Prefix, *InTransform.ToString())
		}
	}
	else if (LoggingOption == LO_Both)
	{
		if (bFormat)
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
			UE_LOG(LogTransform, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *InTransform.GetLocation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sRotation: %s"), NET_MODE_PREFIX, *InTransform.GetRotation().ToString())
			UE_LOG(LogTransform, Warning, TEXT("%sScale: %s"), NET_MODE_PREFIX, *InTransform.GetScale3D().ToString())

			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Scale: ") + InTransform.GetScale3D().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Rotation: ") + InTransform.GetRotation().ToString());
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + InTransform.GetLocation().ToString());

			if (!Prefix.IsEmpty())
				GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
		}
		else
		{
			UE_LOG(LogTransform, Warning, TEXT("%s%s%s"), NET_MODE_PREFIX, *Prefix, *InTransform.ToString())
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + InTransform.ToString());
		}
	}
}

void ULog::Transform(const FTransform& InTransform, const ELoggingOptions LoggingOption, const bool bFormat, const float TimeToDisplay)
{
	Transform(InTransform, "", bFormat, LoggingOption, TimeToDisplay);
}

void ULog::Quat(const FQuat& Quaternion, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + Quaternion.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogQuaternion, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *Quaternion.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogQuaternion, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *Quaternion.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + Quaternion.ToString() + Suffix);
	}
}

void ULog::Quat(const FQuat& Quaternion, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Quat(Quaternion, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Matrix(const FMatrix& InMatrix, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + InMatrix.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMatrix, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *InMatrix.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogMatrix, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *InMatrix.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + InMatrix.ToString() + Suffix);
	}
}

void ULog::Matrix(const FMatrix& InMatrix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Matrix(InMatrix, "", "", LoggingOption, TimeToDisplay);
}

void ULog::Color(const FLinearColor& InColor, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + InColor.ToString() + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *InColor.ToString(), *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *InColor.ToString(), *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + InColor.ToString() + Suffix);
	}
}

void ULog::Color(const FLinearColor& InColor, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	Color(InColor, "", "", LoggingOption, TimeToDisplay);	
}

void ULog::Sphere(const FSphere& Sphere, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Radius: ") + FString::SanitizeFloat(Sphere.W));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + Sphere.Center.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogColor, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *Sphere.Center.ToString())
		UE_LOG(LogColor, Warning, TEXT("%sRadius: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Sphere.W))
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogColor, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *Sphere.Center.ToString())
		UE_LOG(LogColor, Warning, TEXT("%sRadius: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Sphere.W))
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
		
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Radius: ") + FString::SanitizeFloat(Sphere.W));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + Sphere.Center.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
}

void ULog::Box(const FBox& Box, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Max: ") + Box.Max.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Min: ") + Box.Min.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogColor, Warning, TEXT("%sMin: %s"), NET_MODE_PREFIX, *Box.Min.ToString())
		UE_LOG(LogColor, Warning, TEXT("%sMax: %s"), NET_MODE_PREFIX, *Box.Max.ToString())
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogColor, Warning, TEXT("%sMin: %s"), NET_MODE_PREFIX, *Box.Min.ToString())
		UE_LOG(LogColor, Warning, TEXT("%sMax: %s"), NET_MODE_PREFIX, *Box.Max.ToString())
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
		
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Max: ") + Box.Max.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Min: ") + Box.Min.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
}

void ULog::Capsule(const FCapsuleShape& Capsule, const FString& Prefix, const FString& Suffix, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Orientation: ") + Capsule.Orientation.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Length: ") + FString::SanitizeFloat(Capsule.Length));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Radius: ") + FString::SanitizeFloat(Capsule.Radius));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + Capsule.Center.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogColor, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *Capsule.Center.ToString())
		UE_LOG(LogColor, Warning, TEXT("%sRadius: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Capsule.Radius))
		UE_LOG(LogColor, Warning, TEXT("%sLength: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Capsule.Length))
		UE_LOG(LogColor, Warning, TEXT("%sOrientation: %s"), NET_MODE_PREFIX, *Capsule.Orientation.ToString())
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Prefix)
		UE_LOG(LogColor, Warning, TEXT("%sLocation: %s"), NET_MODE_PREFIX, *Capsule.Center.ToString())
		UE_LOG(LogColor, Warning, TEXT("%sRadius: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Capsule.Radius))
		UE_LOG(LogColor, Warning, TEXT("%sLength: %s"), NET_MODE_PREFIX, *FString::SanitizeFloat(Capsule.Length))
		UE_LOG(LogColor, Warning, TEXT("%sOrientation: %s"), NET_MODE_PREFIX, *Capsule.Orientation.ToString())
		UE_LOG(LogColor, Warning, TEXT("%s%s"), NET_MODE_PREFIX, *Suffix)
		
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Suffix);
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Orientation: ") + Capsule.Orientation.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Length: ") + FString::SanitizeFloat(Capsule.Length));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Radius: ") + FString::SanitizeFloat(Capsule.Radius));
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Location: ") + Capsule.Center.ToString());
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix);
	}
}

void ULog::Number_Int_Blueprint(const int32 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogInt(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::Number_Float_Blueprint(const float Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	LogFloat(Number, Prefix, Suffix, NumberSystem, LoggingOption, TimeToDisplay);
}

void ULog::CheckObject(UObject* Object, const FString& Message)
{
	if (!Object)
		ASSERT(Object, Message);
}

void ULog::CheckCondition(const bool bCondition, const FString& Message)
{
	if (!bCondition)
		ASSERT(bCondition, Message);
}

void ULog::CheckNoEntry()
{
	checkNoEntry();
}

void ULog::CheckNoReEntry()
{
	checkNoReentry();
}

void ULog::CheckNoRecursion()
{
	checkNoRecursion();
}

void ULog::UnImplemented()
{
	unimplemented();
}

void ULog::EnsureObject(UObject* Object, const bool bAlwaysEnsure, const FString& Message)
{
	if (Message.IsEmpty())
	{
		if (bAlwaysEnsure)
		{
			ensureAlways(Object != nullptr);
		}
		else
		{
			ensure(Object != nullptr);
		}
	}
	else
	{
		if (bAlwaysEnsure)
		{
			ensureAlwaysMsgf(Object != nullptr, TEXT("Ensure (Object): %s"), *Message);
		}
		else
		{
			ensureMsgf(Object != nullptr, TEXT("Ensure (Object): %s"), *Message);
		}
	}
}

void ULog::EnsureCondition(const bool bCondition, const bool bAlwaysEnsure, const FString& Message)
{
	if (Message.IsEmpty())
	{
		if (bAlwaysEnsure)
		{
			ensureAlways(bCondition);
		}
		else
		{
			ensure(bCondition);
		}
	}
	else
	{
		if (bAlwaysEnsure)
		{
			ensureAlwaysMsgf(bCondition, TEXT("Ensure (Bool): %s"), *Message);
		}
		else
		{
			ensureMsgf(bCondition, TEXT("Ensure (Bool): %s"), *Message);
		}
	}
}

void ULog::LogInt(const int64 Number, const FString& Prefix, const FString& Suffix, const EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	FString FinalNumber;
	switch (NumberSystem)
	{
		case DLNS_Decimal:
			FinalNumber = FString::FromInt(Number);
		break;

		case DLNS_Hex:
			FinalNumber = DecimalToHex(Number);
		break;

		case DLNS_Binary:
			FinalNumber = DecimalToBinary(Number);
		break;

		case DLNS_Octal:
			FinalNumber = DecimalToOctal(Number);
		break;

		case DLNS_Roman:
			FinalNumber = DecimalToRomanNumeral(Number);
		break;
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FString("Hex: ") + DecimalToHex(Number) + Suffix);

	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + FinalNumber + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *FinalNumber, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%s%s"), NET_MODE_PREFIX, *Prefix, *FinalNumber, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + FinalNumber + Suffix);
	}
}

void ULog::LogUInt(const uint64 Number, const FString& Prefix, const FString& Suffix, EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + FString::FromInt(Number) + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%lld%s"), NET_MODE_PREFIX, *Prefix, Number, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%lld%s"), NET_MODE_PREFIX, *Prefix, Number, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + FString::FromInt(Number) + Suffix);
	}
}

void ULog::LogFloat(const float Number, const FString& Prefix, const FString& Suffix, EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + FString::SanitizeFloat(Number) + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%f%s"), NET_MODE_PREFIX, *Prefix, Number, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%f%s"), NET_MODE_PREFIX, *Prefix, Number, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + FString::SanitizeFloat(Number) + Suffix);
	}
}

void ULog::LogLongInt(const long Number, const FString& Prefix, const FString& Suffix, EDebugLogNumberSystems NumberSystem, const ELoggingOptions LoggingOption, const float TimeToDisplay)
{
	if (LoggingOption == LO_Viewport)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + FString::FromInt(Number) + Suffix);
	}
	else if (LoggingOption == LO_Console)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%d%s"), NET_MODE_PREFIX, *Prefix, Number, *Suffix)
	}
	else if (LoggingOption == LO_Both)
	{
		UE_LOG(LogNumber, Warning, TEXT("%s%s%d%s"), NET_MODE_PREFIX, *Prefix, Number, *Suffix)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Settings->InfoColor, NET_MODE_PREFIX + Prefix + FString::FromInt(Number) + Suffix);
	}
}

FString ULog::DecimalToHex(const int64 DecimalNumber)
{
	if (DecimalNumber < 10 && DecimalNumber >= 0)
		return FString::FromInt(DecimalNumber);

#if PLATFORM_64BITS
	int64 Number = DecimalNumber;
#else
	int32 Number = DecimalNumber;
#endif

	//FString BinaryNumber = DecimalToBinary(DecimalNumber);
	//TArray<int32> Zeros;
	//TArray<int32> Ones;

	//for (int32 i = 0; i < BinaryNumber.GetCharArray().Num(); i++)
	//	Zeros.Add(BinaryNumber.Find("0"));

	//for (int32 i = 0; i < BinaryNumber.GetCharArray().Num(); i++)
	//	Ones.Add(BinaryNumber.Find("1"));

	//for (int32 i = 0; i < Zeros.Num(); i++)
	//{
	//	BinaryNumber.RemoveAt(i);
	//	BinaryNumber.InsertAt(i, '1');
	//}

	//for (int32 i = 0; i < Ones.Num(); i++)
	//{
	//	BinaryNumber.RemoveAt(i);
	//	BinaryNumber.InsertAt(i, '0');
	//}

	//BinaryNumber.AppendChar('1');

	//FString BetterResult = BinaryToHex(BinaryNumber);

	bool bIsNegative = false;

	if (DecimalNumber < 0)
	{
		bIsNegative = true;
		Number = -Number;
	}

	std::stringstream SS;
	SS << std::hex << Number;
	FString Result = FString(SS.str().c_str());
	
	if (bIsNegative)
		Result.InsertAt(0, '-');

	//if (bIsNegative)
	//	return BinaryToHex(BinaryNumber);

	return Result.ToUpper();

	//const auto CalculateDigits = [&](int64 Number)
	//{
	//	int64 Digits = 0;
	//	while (Number)
	//	{
	//		Number /= 10;
	//		Digits++;
	//	}

	//	return Digits;
	//};

	//// Char array to store hexadecimal number
	//TCHAR HexadecimalValues[100] = {0};
	//
	//// Counter for hexadecimal number array
	//int32 i = 0;
	//
	//while (DecimalNumber != 0)
	//{
	//	const int32 Remainder = DecimalNumber % 16;
	//
	//	if (Remainder < 10)
	//	{
	//		HexadecimalValues[i++] = Remainder + 48;
	//	}
	//	else
	//	{
	//		HexadecimalValues[i++] = Remainder + 55;
	//	}
	//
	//	//i++;
	//
	//	DecimalNumber /= 16;
	//}
	//
	//FString Result;
	//
	//// Adding hexadecimal values to Result, in reverse order 
	//for (int32 j = i-1; j >= 0 ; j--)
	//	Result.AppendChar(HexadecimalValues[j]);
	//
	//return Result;
}

FString ULog::DecimalToBinary(const int64 DecimalNumber)
{
	if (DecimalNumber == 0)
		return "0";

	if (DecimalNumber == 1)
		return "1";
	
	// Array to store binary numbers
#if PLATFORM_64BITS
	int64 BinaryArray[64];
#else
	int32 BinaryArray[64];
#endif

#if PLATFORM_64BITS
	int64 Number = DecimalNumber;
#else
	int32 Number = DecimalNumber;
#endif

#if PLATFORM_64BITS
	int64 i, j;
#else
	int32 i, j;
#endif
	
	bool bIsNegative = false;

	if (DecimalNumber < 0)
	{
		bIsNegative = true;
		Number = -Number;
	}

	for (i = 0; Number != 0; i++)
	{
		// Store the remainder in binary array
		BinaryArray[i] = Number % 2;
		Number /= 2;
	}

	FString Result;

	// Adding each digit into Result, in reverse order
	for (j = i - 1; j >= 0; j--)
		Result.AppendInt(BinaryArray[j]);

	if (bIsNegative)
		Result.InsertAt(0, '-');

	return Result;
}

FString ULog::DecimalToOctal(const int64 DecimalNumber)
{
	if (DecimalNumber == 0)
		return "0";

#if PLATFORM_64BITS
	int64 OctalArray[64];
#else
	int32 OctalArray[64];
#endif

#if PLATFORM_64BITS
	int64 Number = DecimalNumber;
#else
	int32 Number = DecimalNumber;
#endif

#if PLATFORM_64BITS
	int64 i, j;
#else
	int32 i, j;
#endif

	bool bIsNegative = false;

	if (DecimalNumber < 0)
	{
		bIsNegative = true;
		Number = -Number;
	}
	
	for (i = 0; Number != 0; i++)
	{
		OctalArray[i] = Number % 8;
		Number /= 8;
	}

	FString Result;

	// Adding each digit into Result, in reverse order
	for (j = i - 1; j >= 0; j--)
		Result.AppendInt(OctalArray[j]);

	if (bIsNegative)
		Result.InsertAt(0, '-');

	return Result;
}

FString ULog::DecimalToRomanNumeral(int64 DecimalNumber)
{
	if (DecimalNumber == 0)
		return "N";

	bool bIsNegative = false;
	if (DecimalNumber < 0)
	{
		bIsNegative = true;
		DecimalNumber = -DecimalNumber;
	}

#if PLATFORM_64BITS
	TArray<int64> RomanNumeral_Integers = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
#else
	TArray<int32> RomanNumeral_Integers = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
#endif

	TArray<FString> RomanNumeral_Symbols = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};

#if PLATFORM_64BITS
	int64 i = RomanNumeral_Symbols.Num() - 1;
	int64 Quotient = 0;
#else
	int32 i = RomanNumeral_Symbols.Num() - 1;
	int32 Quotient = 0;
#endif
	
	FString Result;
	while (DecimalNumber > 0)
	{	
		Quotient = DecimalNumber/RomanNumeral_Integers[i];
		DecimalNumber %= RomanNumeral_Integers[i];

		while (Quotient--)
		{
			Result.Append(RomanNumeral_Symbols[i]);
		}

		i--;
	}

	if (bIsNegative)
		Result.InsertAt(0, '-');
	
	return Result;
}
