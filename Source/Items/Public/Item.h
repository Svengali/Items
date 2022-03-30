// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISpudObject.h"
#include "UObject/Object.h"
#include "Item.generated.h"

//-----------------------------------------------------------------------------

UCLASS(BlueprintType)
class ITEMS_API UItemComCfg : public UDataAsset
{
	GENERATED_BODY()

public:

};

USTRUCT(BlueprintType)
struct ITEMS_API FItemCom
{
	GENERATED_USTRUCT_BODY()

	UItemComCfg* Cfg() { return nullptr; }


};


USTRUCT(BlueprintType)
struct ITEMS_API FItemComHolder
{
	GENERATED_USTRUCT_BODY()

	TSharedPtr<FItemCom> Com;
};

/*
template<>
struct TStructOpsTypeTraits<FItemComHolder> : public TStructOpsTypeTraitsBase2<FItemComHolder>
{
	enum
	{
		WithZeroConstructor = false,                         // struct can be constructed as a valid object by filling its memory footprint with zeroes.
		WithNoInitConstructor = false,                         // struct has a constructor which takes an EForceInit parameter which will force the constructor to perform initialization, where the default constructor performs 'uninitialization'.
		WithNoDestructor = false,                         // struct will not have its destructor called when it is destroyed.
		WithCopy = false,
		WithIdenticalViaEquality = false,                         // struct can be compared via its operator==.  This should be mutually exclusive with WithIdentical.
		WithIdentical = false,                         // struct can be compared via an Identical(const T* Other, uint32 PortFlags) function.  This should be mutually exclusive with WithIdenticalViaEquality.
		WithExportTextItem = false,                         // struct has an ExportTextItem function used to serialize its state into a string.
		WithImportTextItem = false,                         // struct has an ImportTextItem function used to deserialize a string into an object of that class.
		WithAddStructReferencedObjects = false,                         // struct has an AddStructReferencedObjects function which allows it to add references to the garbage collector.
		WithSerializer = false,                         // struct has a Serialize function for serializing its state to an FArchive.
		WithStructuredSerializer = false,                         // struct has a Serialize function for serializing its state to an FStructuredArchive.
		WithPostSerialize = false,                         // struct has a PostSerialize function which is called after it is serialized
		WithNetSerializer = false,                         // struct has a NetSerialize function for serializing its state to an FArchive used for network replication.
		WithNetDeltaSerializer = false,                         // struct has a NetDeltaSerialize function for serializing differences in state from a previous NetSerialize operation.
		WithSerializeFromMismatchedTag = false,                         // struct has a SerializeFromMismatchedTag function for converting from other property tags.
		WithStructuredSerializeFromMismatchedTag = false,               // struct has an FStructuredArchive-based SerializeFromMismatchedTag function for converting from other property tags.
		WithPostScriptConstruct = false,                         // struct has a PostScriptConstruct function which is called after it is constructed in blueprints
		WithNetSharedSerialization = false,                         // struct has a NetSerialize function that does not require the package map to serialize its state.
		WithPureVirtual = false,                         // struct has PURE_VIRTUAL functions and cannot be constructed when CHECK_PUREVIRTUALS is true
	};
};
*/

//-----------------------------------------------------------------------------

UCLASS(BlueprintType)
class ITEMS_API UItemCfg : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Item")
	TArray< UItemComCfg* > ComponentConfigs;

};

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item")
	TArray< FItemComHolder > Components;
};

//-----------------------------------------------------------------------------

UCLASS(BlueprintType)
class ITEMS_API UICEquippableCfg : public UItemComCfg
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct ITEMS_API FICEquippable : public FItemCom
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY()
	UICEquippableCfg *Config;
};

//-----------------------------------------------------------------------------
























//-----------------------------------------------------------------------------

UCLASS()
class ITEMS_API UItemSaveObjectCustomData : public UObject, public ISpudObjectCallback
{
	GENERATED_BODY()
public:

	bool bSomeBoolean;
	int SomeInteger;
	FString SomeString;
	float SomeFloat;

	// just for Item read
	bool Peek1Succeeded;
	bool Peek1IDOK;
	bool Peek2Succeeded;
	bool Peek2IDOK;
	bool Skip1Succeeded;
	bool Skip1PosOK;
	bool Skip2Succeeded;
	bool Skip2PosOK;

	static const FString ItemChunkID1;
	static const FString ItemChunkID2;

	virtual void SpudStoreCustomData_Implementation(const USpudState* State, USpudStateCustomData* CustomData) override;
	virtual void SpudRestoreCustomData_Implementation(USpudState* State, USpudStateCustomData* CustomData) override;
};
