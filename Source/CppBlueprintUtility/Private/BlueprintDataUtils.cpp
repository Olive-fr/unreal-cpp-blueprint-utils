
#if WITH_EDITORONLY_DATA
#include "BlueprintDataUtils.h"
#include "Engine/InheritableComponentHandler.h"
#include "Engine/SCS_Node.h"


UObject* FBlueprintDataUtils::GetDefaultObject(UClass* InBlueprintClass, const UStruct* InCppClassStaticClass)
{
	if (const UBlueprint* BlueprintFromClass = UBlueprint::GetBlueprintFromClass(InBlueprintClass))
	{
		const TSubclassOf<UObject> ParentClass = BlueprintFromClass->ParentClass;
		if (ParentClass->IsChildOf(InCppClassStaticClass))
		{
			TSubclassOf<UObject> Class = BlueprintFromClass->GeneratedClass;
			if (Class)
			{
				if (Class->IsValidLowLevel())
				{
					if (UObject* DefaultObject = Class->GetDefaultObject(false))
					{
						return DefaultObject;
					}
				}
			}
		}
	}
	return nullptr;
}

UActorComponent* FBlueprintDataUtils::GetClassComponentTemplate(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass, FName& OutComponentName, const FName InComponentName /*= NAME_None*/)
{
	if (IsValid(InActorClass) && IsValid(InComponentClass))
	{
		UBlueprintGeneratedClass* CurrentBPClass = Cast<UBlueprintGeneratedClass>(InActorClass);
		while (CurrentBPClass)
		{
			// Not all BP's have an SCS (apparently)
			if (CurrentBPClass->SimpleConstructionScript)
			{
				if (InComponentName != NAME_None)
				{
					// Find Matching Node
					const USCS_Node* FoundNode = CurrentBPClass->SimpleConstructionScript->FindSCSNode(InComponentName);
					if (FoundNode && FoundNode->ComponentClass->IsChildOf(InComponentClass))
					{
						if (UActorComponent* FoundComponent = FoundNode->GetActualComponentTemplate(CurrentBPClass))
						{
							OutComponentName = InComponentName;
							return FoundComponent;
						}
					}
				}
				else
				{
					// No specified name, just return first component class that matches.
					const TArray<USCS_Node*>& AllSCSNodes = CurrentBPClass->SimpleConstructionScript->GetAllNodes();
					for (const USCS_Node* NodeItr : AllSCSNodes)
					{
						if (NodeItr->ComponentClass->IsChildOf(InComponentClass))
						{
							OutComponentName = NodeItr->GetVariableName();
							return NodeItr->GetActualComponentTemplate(CurrentBPClass);
						}
					}
				}
			}

			// Didn't find an explicit entry in this class, check inherited component overrides.
			if (UInheritableComponentHandler* Handler = CurrentBPClass->GetInheritableComponentHandler(false))
			{
				if (InComponentName != NAME_None)
				{
					const FComponentKey FoundKey = Handler->FindKey(InComponentName);
					if (FoundKey.IsValid())
					{
						if (UActorComponent* FoundComponent = Handler->GetOverridenComponentTemplate(FoundKey))
						{
							OutComponentName = InComponentName;
							return FoundComponent;
						}
					}
				}
				else
				{
					TArray<FComponentOverrideRecord>::TIterator InheritedComponentItr = Handler->CreateRecordIterator();
					for (InheritedComponentItr; InheritedComponentItr; ++InheritedComponentItr)
					{
						const FComponentOverrideRecord& Record = *InheritedComponentItr;
						if (Record.ComponentClass->IsChildOf(InComponentClass) && Record.ComponentTemplate)
						{
							OutComponentName = Record.ComponentKey.GetSCSVariableName();
							return Record.ComponentTemplate;
						}
					}
				}
			}

			// Wasn't found in this class or it's inherited vars. Check the Super BP Class
			CurrentBPClass = Cast<UBlueprintGeneratedClass>(CurrentBPClass->GetSuperClass());
		}

		// If we get this far, didn't find a Blueprint template. Get the component properties from the native CDO if possible.
		if (const AActor* CDO = InActorClass.GetDefaultObject())
		{
			if (InComponentName == NAME_None)
			{
				if (UActorComponent* FoundComponent = CDO->FindComponentByClass(InComponentClass))
				{
					OutComponentName = InComponentName;
					return FoundComponent;
				}
			}
			else
			{
				for (UActorComponent* ComponentItr : CDO->GetComponents())
				{
					if (ComponentItr->GetFName() == InComponentName && ComponentItr->GetClass()->IsChildOf(InComponentClass))
					{
						OutComponentName = ComponentItr->GetFName();
						return ComponentItr;
					}

					return nullptr;
				}
			}
		}
	}

	OutComponentName = NAME_None;
	return nullptr;
}

#endif
