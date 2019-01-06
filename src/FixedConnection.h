
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef FIXED_CONNECTION_H
#define FIXED_CONNECTION_H

#include <vector>
#include <array>

#include <boost/asio/basic_stream_socket.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "GlobalBoostAsio.h"
#include "Error.h"

/*
	Warning:
		when packet whit header size equal to 0 arrive, it mean, connection should be closed
*/

namespace ICon
{
	class FixedConnection final
	{
	private:
		
		bool isValid;
		
		boost::asio::ip::tcp::socket socket;
		boost::asio::socket_base::bytes_readable command;
		
		std::vector < std::vector < unsigned char > > buffers;
		std::vector < unsigned char > receiveBuffer;
		
		const std::vector < unsigned char > constReferenceBuffer;
		
		
		friend class DeepServer;
		
		unsigned GetUnreceivedBytes();
		
		void UpdateReceiveBuffer();
		
	public:
		
		const static unsigned maxBufferSize = 2147483647;
		
		bool IsValid() const;
		
		unsigned Connect( const char * ip, const unsigned short port );
		
		void Receive();
		void ReceiveLock();
		
		unsigned CountReceivedMessages() const;
		const std::vector < unsigned char > & GetMessageLock();
		const std::vector < unsigned char > & GetMessage();
		void PopMessage( unsigned count = 1 );
		
		unsigned long long Send( const void * buffer, const unsigned bytes );		// return bytes sent
		
		void ErrorClose();
		void Close();
		
		FixedConnection();
		~FixedConnection();
	};
};

#endif

