
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef HIGH_LAYER_SOCKET_H
#define HIGH_LAYER_SOCKET_H

#include "GlobalBoostAsio.h"
#include "Error.h"
#include "LoopBuffer.h"

#include <boost/asio/io_service.hpp>

/*
	Warning:
		when packet whit header size equal to 0 arrive, it mean, connection should be closed
*/

namespace ICon
{
	class HighLayerSocket final
	{
	private:
		
		LoopBuffer buffer;
		
		bool isValid;
		
		/*boost::asio::ip::tcp::socket*/ void * socket;
		/*boost::asio::socket_base::bytes_readable*/ void * command;
		
		
		friend class Server;
		
		unsigned GetUnreceivedBytes();
		void ReceiveClose();
		
	public:
		
		const static unsigned maxMessageSize = (1024*1024*1024) - 1;
		
		bool IsValid() const;
		
		unsigned Connect( const char * ip, const unsigned short port );
		
		void Receive( bool lockForAnyBytes = false );
		
		const LoopBuffer & AccessBuffer() const;
		
		bool IsAllMessageAvailable();
		bool IsMessageAvailable();
		unsigned long long GetNextMessageLengthLock();
		unsigned long long GetNextMessageLength() const;		// only if this->IsMessageAvailable() == true else return 0
		// length - maximum bytes to receive from message
		unsigned long long GetMessage( void * dst, unsigned long long length, unsigned long long offset = 0 );			// returns received bytes
		unsigned long long GetMessageLock( void * dst, unsigned long long length, unsigned long long offset = 0 );		// returns received bytes
		void PopMessage( void * dst = nullptr );		// dst != nullptr -> dst is capable to store bytes amount of this->GetNextMessageLength() ; pop is effective only when this->IsAllMessageAvailable() == true
		void GetPopMessageLock( void * dst );			// dst is capable to store bytes amount of this->GetNextMessageLength()
		
		
		unsigned long long Send( const void * buffer, const unsigned bytes );		// return bytes sent
		unsigned long long Send( const void ** buffer, const unsigned * bytes, const unsigned buffers );		// return bytes sent
		
		void ErrorClose();
		void Close();
		
		HighLayerSocket();
		~HighLayerSocket();
	};
};

#endif

