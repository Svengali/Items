// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

const FString UItemSaveObjectCustomData::ItemChunkID1("chk1");
const FString UItemSaveObjectCustomData::ItemChunkID2("chk2");

void UItemSaveObjectCustomData::SpudStoreCustomData_Implementation(const USpudState* State,
	USpudStateCustomData* CustomData)
{
	// store as a custom chunk, nested
	CustomData->BeginWriteChunk(ItemChunkID1);
	CustomData->WriteByte((uint8)bSomeBoolean);

	// Nested
	CustomData->BeginWriteChunk(ItemChunkID2);
	CustomData->WriteInt(SomeInteger);
	CustomData->WriteString(SomeString);
	CustomData->EndWriteChunk(ItemChunkID2);


	CustomData->WriteFloat(SomeFloat);
	CustomData->EndWriteChunk(ItemChunkID1);
	
	//Bad
	//FICIntegrity integrity  { 100.0f };




}

void UItemSaveObjectCustomData::SpudRestoreCustomData_Implementation(USpudState* State,
	USpudStateCustomData* CustomData)
{
	// This method includes a lot of double checking, you'd normally just mirror the store in read form
	// However for the Item we're going to go back & forth, peeking and skipping as well to make sure that works.
	const uint64 OrigPos = CustomData->GetUnderlyingArchive()->Tell();
	uint64 PostNestedPos = 0;

	Peek1Succeeded = false;
	Peek1IDOK = false;
	Peek2Succeeded = false;
	Peek2IDOK = false;
	Skip1Succeeded = false;
	Skip1PosOK = false;
	Skip2Succeeded = false;
	Skip2PosOK = false;
	
	// Item peeking as well while we're doing it
	FString PeekChunkID;
	Peek1Succeeded = CustomData->PeekChunk(PeekChunkID);
	Peek1IDOK = PeekChunkID == ItemChunkID1;
	
	if (CustomData->BeginReadChunk(ItemChunkID1))
	{
		uint8 Byte;
		CustomData->ReadByte(Byte);
		bSomeBoolean = (bool)Byte;

		// Nested
		Peek2Succeeded = CustomData->PeekChunk(PeekChunkID);
		Peek2IDOK = PeekChunkID == ItemChunkID2;

		if (CustomData->BeginReadChunk(ItemChunkID2))
		{
			CustomData->ReadInt(SomeInteger);
			CustomData->ReadString(SomeString);
			CustomData->EndReadChunk(ItemChunkID2);
			PostNestedPos = CustomData->GetUnderlyingArchive()->Tell();
		}

		CustomData->ReadFloat(SomeFloat);
		CustomData->EndReadChunk(ItemChunkID1);
	}

	// Record end pos for more Iteming
	const uint64 EndPos = CustomData->GetUnderlyingArchive()->Tell();

	// Now Item skipping
	CustomData->GetUnderlyingArchive()->Seek(OrigPos);

	// Should be able to skip entire 1st chunk & skip everything
	Skip1Succeeded = CustomData->SkipChunk(ItemChunkID1);
	Skip1PosOK = CustomData->GetUnderlyingArchive()->Tell() == EndPos;
	
	// Back to start so we can Item skiping nested only
	CustomData->GetUnderlyingArchive()->Seek(OrigPos);

	// Read outer & discard
	if (CustomData->BeginReadChunk(ItemChunkID1))
	{
		uint8 Byte;
		CustomData->ReadByte(Byte);

		// Skip nested
		Skip2Succeeded = CustomData->SkipChunk(ItemChunkID2);
		Skip2PosOK = CustomData->GetUnderlyingArchive()->Tell() == PostNestedPos;
	}

	// Jump to end juat for consistency
	CustomData->GetUnderlyingArchive()->Seek(EndPos);

}
