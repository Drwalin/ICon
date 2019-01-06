
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef ERROR_H
#define ERROR_H

#include <vector>
#include <string>
#include <iostream>

#include <mutex>

namespace ICon
{
	class Error final
	{
	public:
		
		std::string message;
		void * ptr;
		unsigned code;
		
		static void PrintWhenPushed();
		static void DoNotPrintWhenPushed();
		
		static void Push( unsigned code, const std::string & message, void * ptr = nullptr );
		static void Push( unsigned code );
		static Error Pop();
		
		operator unsigned() const;
		
		void Print( std::ostream & stream = std::cerr );
		
		Error & operator = ( const Error & other );
		
		Error( unsigned code_, const std::string & message_, void * ptr_ );
		Error();
		~Error();
		
		static std::string GetErrorCodeString( unsigned code );
		
	// error codes
	public:
		
		enum Code
		{
			none =		0,
			
			invalidSocket =				1,
			invalidServer =				2,
			noConnection =				3,
			failedToAccept =			4,
			failedToConnect =			5,
			invalidIpAddress =			6,
			invalidBuffer =				7,
			failedToInitIoService =		8,
			tryingToInitSecondTime =	9,
			
			tryingToRunAcceptWhileAcceptNoLockIsRunning =		10,
			tryingToRunSecondInstancOfAcceptNoLock =			11,
			
			tryingToAccessUnexistingBuffer =			12,
			tryingToDeInintUnexistingContext =			13,
			tryingToAcceptUsingClosedServer =			14,
			tryingToAccessEmptyErrorQueue =				15,
			tryingToPopMoreBuffersThanExist = 			16,
			tryingToGetBufferFromInvalidSocket =		17,
			tryingToSendDataByInvalidConnectino =		18,
			tryingToSendInvalidDataSize =				19,
			tryingToSendInvalidDataPointer =			20,
			connectionClosedByErrorWhileSendingData =	21,
			tryingToAcceptToUnallocatedConnection =		22,
			tryingToReceiveFromInvalidConnection =		23,
			connectionBrokenWhileReceiving =			24,
			packetWithInvalidPointerSizeArrived =		25,
			tryingToReceiveToInvalidType =				26,
			
			failedToReceiveLock =					27,
			failedToRestoreMessage =				28,
			tryingToSendInvalidBuffersArray =		29,
			tryingToSendThroughInvalidConnection =	30,
			tryingToSendThroughInvalidConnection =	31,
			tryingToReadFromInvalidConnection =		32,
			failedToReceiveBuffer =					33,
			receivedInvalidBuffer =					34,
			tryingToReadFromBufferOfTypeRaw =		35,
			
			failedToGetMessageLockWhichReturnedConstReference = 36,
			
			unknown =		65565
		};
	};
	
	extern std::mutex globalErrorQueueModificationMutex;
	extern std::vector < Error > globalErrorQueue;
	extern bool printErrorWhenPushed;
};

#endif

