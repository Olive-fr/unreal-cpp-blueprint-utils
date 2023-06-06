#pragma once

#if WITH_EDITORONLY_DATA

#include "Editor.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "UObject/UnrealTypePrivate.h" 	
 	

#include "UObject/UnrealType.h"
#endif
#include "BlueprintCompilationUtils.generated.h"

UCLASS()
class CPPBLUEPRINTUTILITY_API UBlueprintCompilationUtils : public UObject
{
	GENERATED_BODY()
	
#if WITH_EDITORONLY_DATA
private:
	static TMap<const FName, UObject*> LastCompiledObjectIsolated;
	static UObject* LastCompiledObject;
	
public:
	// static bool IsCompiled(const UObject* ObjectToTest, bool ResetIfTrue);
	static bool IsCompiled(const FName IsolationContext, const UObject* InObjectToTest, bool ResetIfTrue);
	
	static void RegisterPreCompileEvent()
	{
		static bool bAlreadyExecuted = false;
		if (!bAlreadyExecuted)
		{
			if (GEditor)
			{
				bAlreadyExecuted = true;
				GEditor->OnBlueprintPreCompile().AddLambda([](UBlueprint* Blueprint)
				{
					LastCompiledObject = Blueprint;
					LastCompiledObjectIsolated = TMap<const FName, UObject*>();
				});
			}
		}
	}
#endif
};

