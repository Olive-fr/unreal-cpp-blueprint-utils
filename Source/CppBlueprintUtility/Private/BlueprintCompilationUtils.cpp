#if WITH_EDITORONLY_DATA
#include "BlueprintCompilationUtils.h"

UObject* UBlueprintCompilationUtils::LastCompiledObject = nullptr;
TMap<const FName, UObject*> UBlueprintCompilationUtils::LastCompiledObjectIsolated = TMap<const FName, UObject*>();

bool UBlueprintCompilationUtils::IsCompiled(const FName IsolationContext, const UObject* InObjectToTest, bool ResetIfTrue)
{
	
#if WITH_EDITOR
	
	if (GEditor && GEditor->PlayWorld && GEditor->PlayWorld->IsPlayInEditor())
	{
		return false;
	}
	
	UObject* SavedObjectToTest = nullptr;
	if (LastCompiledObjectIsolated.Contains(IsolationContext))
	{
		UObject** ObjectToTestPtr = LastCompiledObjectIsolated.Find(IsolationContext);
		if (ObjectToTestPtr)
		{
			SavedObjectToTest = *ObjectToTestPtr;
		}
	} else
	{
		LastCompiledObjectIsolated.Add(IsolationContext, LastCompiledObject);
		SavedObjectToTest = LastCompiledObject;
	}
	
	bool bResult = !SavedObjectToTest && !InObjectToTest && SavedObjectToTest == InObjectToTest;

	
	if (!bResult)
	{
		if (SavedObjectToTest && InObjectToTest)
		{
			FString LastCompiledObjectName = SavedObjectToTest->GetName();
			FString ObjectToTestName = InObjectToTest->GetName();
			if (LastCompiledObjectName.Len() > 0 && ObjectToTestName.Len()>0)
			{
				bResult = ObjectToTestName.Contains(LastCompiledObjectName);
			}
		}
	}
	
	if (ResetIfTrue && bResult)
	{
		LastCompiledObjectIsolated.Emplace(IsolationContext, nullptr);
	}
	return bResult;
#else
	return false;
#endif
	
}

#endif
