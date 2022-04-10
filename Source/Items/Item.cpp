// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "SimplexNoiseBPLibrary.h"

#include <chrono>

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

auto timeFuncInvocation =
[](auto&& func, auto&&... params) {
	// get time before function invocation
	const auto& start = std::chrono::high_resolution_clock::now();
	// function invocation using perfect forwarding
	std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);
	// get time after function invocation
	const auto& stop = std::chrono::high_resolution_clock::now();
	return (stop - start) / 100000/*largeNumber*/;
};



void UItemTest::TestSimplex()
{

	const int timeTest = (float)timeFuncInvocation([]() {
		
		FPlatformProcess::Sleep(1.0f);

	}).count();

	UE_LOG(LogTemp, Warning, TEXT("Sleep time %.2f"), timeTest * 0.1f );

	const float invSize = 1 / 20.0f;
	const int cubeEdgeSize = 1024;
	const int cubeEdgeSizeSqr = cubeEdgeSize * cubeEdgeSize;

	float total = 0;


	const int time = (float)timeFuncInvocation([&]() {
		for (int z = 0; z < cubeEdgeSize; ++z)
		{
			for (int y = 0; y < cubeEdgeSize; ++y)
			{
				for (int x = 0; x < cubeEdgeSize; ++x)
				{
					const float fx = static_cast<float>(x) * invSize;
					const float fy = static_cast<float>(y) * invSize;
					const float fz = static_cast<float>(z) * invSize;

					const float index = (fz * cubeEdgeSizeSqr + fy * cubeEdgeSize + fx);


					//total += USimplexNoiseBPLibrary::SimplexNoise3D( fx, fy, fz );
					
					const float singleVal = USimplexNoiseBPLibrary::SimplexNoise1D(index);

					total += singleVal;
				}
			}
		}
		}).count();


	UE_LOG(LogTemp, Warning, TEXT("Time for simplex %.2f"), time * 0.1f);
		
	const float singleTestCall = USimplexNoiseBPLibrary::SimplexNoise1D( 321.3f );

	UE_LOG(LogTemp, Warning, TEXT("Single Test Call %.2f"), singleTestCall);

	UE_LOG( LogTemp, Warning, TEXT("Value Total %.2f"), total );


}
