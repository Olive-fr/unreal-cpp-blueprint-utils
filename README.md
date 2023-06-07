# Why
The goal with this really small library is to allow an actor to compute/modify/add/remove data during the bluprint compilation process. This code is used in a game so it work in my case with Unreal 5.1.1. It's not really optimized for now but it will in a near future.

# Setup
This library need to add its referencing during the startup module method.

In your Module base class, you must have a StartupModule method. If you don't have this method, that's mean you haven't still setup your project as a module. I let you follow this page to do this before: https://docs.unrealengine.com/5.1/en-US/how-to-make-a-gameplay-module-in-unreal-engine/

Next, you need to add the dependency to your ProjectName.Build.cs, the dependency name is "CppBlueprintUtility".
```csharp
public class PorjectName : ModuleRules
{
	public PorjectName(ReadOnlyTargetRules Target) : base(Target)
	{
	    //...
		PublicDependencyModuleNames.AddRange(new string[] { /*...*/, "CppBlueprintUtility", /*...*/});
		//...
	}
}
```

Then in your StartupModule method, add this line
```cpp
UBlueprintCompilationUtils::RegisterPreCompileEvent();
```


And that's all, you are ready to use the library

# Usage Exemples
## Handle the compilation time
### With an Actor
Override the OnConstruction method and use the lib

In your Actor header: 
```cpp
protected: 
	virtual void OnConstruction(const FTransform& Transform) override;
```

In your Actor cpp:
```cpp
void AYourActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    
    ...
    
    #if WITH_EDITOR
    if (UBlueprintCompilationUtils::IsCompiled(GetFName(), this, true))
    {
        // The code you want to execute while compilation
    }
    #endif
    
    ...
}
```

### With an ActorComponent
Override the OnRegister method and use the lib

In your ActorComponent header
```cpp
virtual void OnRegister() override;
```

In your ActorComponent cpp
```cpp
void UObstacleBaseComponent::OnRegister()
{
	Super::OnRegister();

    ...

#if WITH_EDITOR
	if (UBlueprintCompilationUtils::IsCompiled(GetFName(), GetOwner(), true))
	{
        // The code you want to execute while compilation
	}
#endif
    
    ...
}
```

## Add baked values in your Object
### With an Actor
Make sure to define a UPROPERTY you want to persist in your object.
In your ActorComponent header
```cpp
public:
	UPROPERTY(EditAnywhere, Category = "Generated numbers")
	TArray<int32> Fibonacci;
```

Then, you can compute your value by injecting a code like this in place of the previous section where it's writed "The code you want to execute while compilation"

In your ActorComponent cpp
```cpp
if (RootComponent)
{
    // We need to get the DefaultSubObject to be able to persit our new values
    if (UObject* DefaultSubObject =
        FBlueprintDataUtils::GetDefaultObject(GetClass(), StaticClass()))
    {			
        // Compute the value to be persisted
        TArray<int32> BakedFibonacci = TArray<int32>();
		BakedFibonacci.Add(1);
		BakedFibonacci.Add(2);
		BakedFibonacci.Add(3);
		BakedFibonacci.Add(5);
		BakedFibonacci.Add(8);
		BakedFibonacci.Add(13);
		BakedFibonacci.Add(21);

		const FName VariableName = "Fibonacci";
		// Persist the new value
		FBlueprintDataUtils::SetArrayProperty(DefaultSubObject, VariableName, BakedFibonacci);
    }
}
```

### With an ActorComponent
Make sure to define a UPROPERTY you want to persist in your object.
```cpp
UPROPERTY(EditAnywhere, Category = "House number")
int8 HouseNumber;
```

Then, you can compute your value by injecting a code like this in place of the previous section where it's writed "The code you want to execute while compilation"
```cpp
if (const AActor* Actor = GetOwner())
{
    FName ComponentName;
    
    // We need to get the ActorComponentTemplate to be able to persit our new values
    if (UActorComponent* ClassComponentTemplate = FBlueprintDataUtils::GetClassComponentTemplate(
        Actor->GetClass(), GetClass(), ComponentName, GetFName()))
    {
        // Persist the value in the template, and compute the value in a dedicated function
        FBlueprintDataUtils::SetInt8Property(ClassComponentTemplate, TEXT("HouseNumber"),
                                             ComputeHouseNumber(GetComponentLocation()));
    }
}
```

# Disclaimer
All kind of variables aren't implemented yet, you can add the missing type if you want and send me a pull request, just test the code on your side please.