
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef HIGH_LAYER_SOCKET_H
#define HIGH_LAYER_SOCKET_H

#include <vector>
#include <array>

#include "GlobalBoostAsio.h"
#include "Error.h"
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
		
		bool isValid;
		
		/*boost::asio::ip::tcp::socket*/ void * socket;
		/*boost::asio::socket_base::bytes_readable*/ void * command;
		
		std::vector < std::vector < unsigned char > > buffers;
		std::vector < unsigned char > receiveBuffer;
		
		const std::vector < unsigned char > constReferenceBuffer;
		
		
		friend class DeepServer;
		
		unsigned GetUnreceivedBytes();
		
		void UpdateReceiveBuffer();
		
	public:
		
		bool IsReferencingToNothing( const std::vector < unsigned char > & var ) const;
		
		const static unsigned maxBufferSize = 2147483647;
		
		bool IsValid() const;
		
		unsigned Connect( const char * ip, const unsigned short port );
		
		void Receive();
		void ReceiveLock( bool doNotThrowErrorBecauseOfClosingSequence = false );
		
		unsigned CountReceivedMessages() const;
		const std::vector < unsigned char > & GetMessageLock();
		const std::vector < unsigned char > & GetMessage();
		void PopMessage( unsigned count = 1 );
		
		unsigned long long Send( const void * buffer, const unsigned bytes );		// return bytes sent
		unsigned long long Send( const void ** buffer, const unsigned * bytes, const unsigned buffers );		// return bytes sent
		
		void ErrorClose();
		void Close();
		
		HighLayerSocket();
		~HighLayerSocket();
	};
};

#endif

