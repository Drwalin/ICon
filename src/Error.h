
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
		unsigned line;
		std::string fileName;
		
		static void PrintWhenPushed();
		static void DoNotPrintWhenPushed();
		
		static void Push( unsigned code, unsigned line, std::string fileName, const std::string & message, void * ptr = nullptr );
		static void Push( unsigned code, unsigned line, std::string fileName );
		static Error Pop();
		
		operator unsigned() const;
		
		void Print( std::ostream & stream = std::cerr );
		
		Error & operator = ( const Error & other );
		
		Error( unsigned code_, unsigned line_, std::string fileName_, const std::string & message_, void * ptr_ );
		Error( const Error & other );
		Error();
		~Error();
		
		static std::string GetErrorCodeString( unsigned code );
		
	// error codes
	public:
		
		enum Code
		{
			none =		0,
			
			failedToReceiveToValidType =	0x1001,
			failedToRestoreMessage =		0x1002,
			failedToReceiveBuffer =			0x1003,
			failedToInitIoService =			0x1004,
			failedToReceiveLock =			0x1005,
			failedToConnect =				0x1006,
			failedToAccept =				0x1007,
			
			tryingToRunAcceptWhileAcceptNoLockIsRunning =		0x2001,
			tryingToGetBufferFromInvalidHighSocketLayer =		0x2002,
			tryingToAcceptToUnallocatedHighSocketLayer =		0x2003,
			tryingToReceiveFromInvalidHighLayerSocket =			0x2004,
			tryingToSendDataByInvalidHighSocketLayer =			0x2005,
			tryingToRunSecondInstancOfAcceptNoLock =			0x2006,
			tryingToSendThorughInvalidConnection =				0x2007,
			tryingToReceiveFromInvalidConnection =				0x2008,
			tryingToSendThroughInvalidConnection =				0x2009,
			tryingToReadFromBufferNotOfTypeRaw =				0x200A,
			tryingToReadFromInvalidConnection =					0x200B,
			tryingToDeInintUnexistingContext =					0x200C,
			tryingToAcceptUsingClosedServer =					0x200D,
			tryingToPopMoreBuffersThanExist = 					0x200E,
			tryingToSendInvalidBuffersArray =					0x200F,
			tryingToSendInvalidDataPointer =					0x2010,
			tryingToAccessEmptyErrorQueue =						0x2011,
			tryingToReceiveToInvalidType =						0x2012,
			tryingToSendInvalidDataSize =						0x2013,
			
			highLayerSocketGetMessageReturnedConstReference =	0x3001,
			connectionClosedByErrorWhileSendingData =			0x3002,
			connectionBrokenWhileReceiving =					0x3003,
			receivedInvalidBuffer =								0x3004,
			
			failedToGetMessageLockWhichReturnedConstReference = 0x4001,
			
			
			
			
			unknown =		0xFFFF
		};
	};
	
	extern std::mutex globalErrorQueueModificationMutex;
	extern std::vector < Error > globalErrorQueue;
	extern bool printErrorWhenPushed;
};

#endif

