#pragma once
#include "ObjectEditorUtils.h"

#if WITH_EDITORONLY_DATA
#include "CoreMinimal.h"
#include "UObject/UnrealType.h"

class CPPBLUEPRINTUTILITY_API FBlueprintDataUtils
{
public:
	/**
	 * @param InBlueprintClass Blueprint class, May be get by GetClass() inside a cpp parent function. This will get the class of your blueprint implementation.  
	 * @param InCppClassStaticClass Cpp parent class, May be get by YourCppParentClass::StaticClass()
	 * 
	 * @return If GeneratedClass and DefaultObject are both get, it will return the DefaultObject, nullptr otherwise
	 */
	static UObject* GetDefaultObject(UClass* InBlueprintClass, const UStruct* InCppClassStaticClass);

	/**
	 * Gets the template component on the actor class with the given component class.
	 * Supports Blueprint Classes
	 *
	 * @param InActorClass			The Actor Blueprint/Native Class
	 * @param InComponentClass		The expected type of actor component.
	 * @param InComponentName		If specified, makes the search faster and only includes components with the given name.
	 */
	static UActorComponent* GetClassComponentTemplate(const TSubclassOf<AActor> InActorClass,
	                                                  const TSubclassOf<UActorComponent> InComponentClass,
	                                                  FName& OutComponentName, const FName InComponentName = NAME_None);


	/**
	 * @param InDefaultObject The default object of the blueprint, this is the base object. You can get it through FBlueprintUtils::GetDefaultObject()
	 * @param InPropertyName The name of your property.
	 * 
	 * @return The array you want. nullptr if the field don't exist in the class, or if a problem occur while getting the array
	 */
	template <typename TInnerType>
	static TArray<TInnerType>* GetArrayProperty(UObject* InDefaultObject, FName InPropertyName)
	{
		const TSubclassOf<UObject> GeneratedClass = InDefaultObject->GetClass();
		if (FProperty* Property = GeneratedClass->FindPropertyByName(InPropertyName))
		{
			if (const FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
			{
				FScriptArray* PropertyValue_InContainer = ArrayProperty->GetPropertyValuePtr_InContainer(
					InDefaultObject);
				void* PropertyValue_InContainerDereference = PropertyValue_InContainer;
				return static_cast<TArray<TInnerType>*>(PropertyValue_InContainerDereference);
			}
		}
		return nullptr;
	}

	/**
	 * @param InDefaultObject The default object of the blueprint, this is the base object. You can get it through FBlueprintUtils::GetDefaultObject()
	 * @param InPropertyName The name of your property.
	 * @param InNewValue The new value of the array
	 * 
	 * @return True if the value is correctly set, false otherwise
	 */
	template <typename TInnerType>
	static bool SetArrayProperty(UObject* InDefaultObject, FName InPropertyName, TArray<TInnerType> InNewValue)
	{
		const TSubclassOf<UObject> GeneratedClass = InDefaultObject->GetClass();
		if (FProperty* Property = GeneratedClass->FindPropertyByName(InPropertyName))
		{
			if (const FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
			{
				if (const void* ArrayPtr = ArrayProperty->ContainerPtrToValuePtr<void>(InDefaultObject))
				{
					FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayPtr);
					ArrayHelper.EmptyAndAddUninitializedValues(InNewValue.Num());
					for (int32 i = 0; i < InNewValue.Num(); i++)
					{
						ArrayProperty->Inner->SetValue_InContainer(ArrayHelper.GetRawPtr(i), &InNewValue[i]);
					}
					return true;
				}
			}
		}
		return nullptr;
	}

	/**
	 * @param InDefaultObject The default object of the blueprint, this is the base object. You can get it through FBlueprintUtils::GetDefaultObject()
	 * @param InPropertyName The name of your property.
	 * @param InNewValue The new value of the property
	 * 
	 * @return True if the value is correctly set, false otherwise
	 */
	static bool SetBoolProperty(UObject* InDefaultObject, FName InPropertyName, bool InNewValue)
	{
		const TSubclassOf<UObject> GeneratedClass = InDefaultObject->GetClass();
		if (FProperty* Property = GeneratedClass->FindPropertyByName(InPropertyName))
		{
			if (const FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
			{
				BoolProperty->SetPropertyValue_InContainer(InDefaultObject, InNewValue);
				return true;
			}
		}
		return false;
	}


	/**
	 * @param InDefaultObject The default object of the blueprint, this is the base object. You can get it through FBlueprintUtils::GetDefaultObject()
	 * @param InPropertyName The name of your property.
	 * @param InNewValue The new value of the property
	 * 
	 * @return True if the value is correctly set, false otherwise
	 */
	static bool SetIntProperty(UObject* InDefaultObject, FName InPropertyName, int InNewValue)
	{
		const TSubclassOf<UObject> GeneratedClass = InDefaultObject->GetClass();
		if (FProperty* Property = GeneratedClass->FindPropertyByName(InPropertyName))
		{
			if (const FIntProperty* IntProperty = CastField<FIntProperty>(Property))
			{
				IntProperty->SetPropertyValue_InContainer(InDefaultObject, InNewValue);
				return true;
			}
		}
		return false;
	}




	/**
	 * @param InDefaultObject The default object of the blueprint, this is the base object. You can get it through FBlueprintUtils::GetDefaultObject()
	 * @param InPropertyName The name of your property.
	 * @param InNewValue The new value of the property
	 * 
	 * @return True if the value is correctly set, false otherwise
	 */
	static bool SetFStringProperty(UObject* InDefaultObject, FName InPropertyName, FString InNewValue)
	{
		const TSubclassOf<UObject> GeneratedClass = InDefaultObject->GetClass();
		if (FProperty* Property = GeneratedClass->FindPropertyByName(InPropertyName))
		{
			if (const FStrProperty* StrProperty = CastField<FStrProperty>(Property))
			{
				StrProperty->SetPropertyValue_InContainer(InDefaultObject, InNewValue);
				return true;
			}
		}
		return false;
	}

	/**
	 * @param InDefaultObject The default object of the blueprint, this is the base object. You can get it through FBlueprintUtils::GetDefaultObject()
	 * @param InPropertyName The name of your property.
	 * @param InNewValue The new value of the property
	 * 
	 * @return True if the value is correctly set, false otherwise
	 */
	static bool SetInt8Property(UObject* InDefaultObject, FName InPropertyName, int8 InNewValue)
	{
		const TSubclassOf<UObject> GeneratedClass = InDefaultObject->GetClass();
		if (FProperty* Property = GeneratedClass->FindPropertyByName(InPropertyName))
		{
			if (const FInt8Property* IntProperty = CastField<FInt8Property>(Property))
			{
				IntProperty->SetPropertyValue_InContainer(InDefaultObject, InNewValue);
				return true;
			}
		}
		return false;
	}
};

#endif
