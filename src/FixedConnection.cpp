
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef FIXED_CONNECTION_CPP
#define FIXED_CONNECTION_CPP

#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>

#include "GlobalBoostAsio.h"
#include "FixedConnection.h"
#include "Error.h"

#include <chrono>
#include <thread>

namespace ICon
{
	unsigned FixedConnection::GetUnreceivedBytes()
	{
		if( this->IsValid() )
		{
			return this->command.get();
		}
		return 0;
	}
	
	bool FixedConnection::IsValid() const
	{
		return this->isValid;
	}
	
	unsigned FixedConnection::Connect( const char * ip, const unsigned short port )
	{
		boost::system::error_code ec;
		this->socket.connect( boost::asio::ip::tcp::endpoint( boost::asio::ip::address::from_string(ip), port ), ec );
		if( ec )
		{
			ICon::Error::Push( ICon::Error::Code::failedToConnect );
			return ICon::Error::Code::failedToConnect;
		}
		this->isValid = true;
		this->socket.io_control( this->command );
		return ICon::Error::none;
	}
	
	void FixedConnection::UpdateReceiveBuffer()
	{
		while( this->receiveBuffer.size() >= sizeof(unsigned) )
		{
			unsigned long long bytesToReceive = unsigned( *( (unsigned*) &(this->receiveBuffer.front()) ) );
			if( bytesToReceive > 0 && bytesToReceive < maxBufferSize )
			{
				if( this->receiveBuffer.size() >= bytesToReceive + sizeof(unsigned) )
				{
					this->buffers.emplace_back( this->receiveBuffer.begin() + sizeof(unsigned), this->receiveBuffer.begin() + sizeof(unsigned) + bytesToReceive );
					this->receiveBuffer.erase( this->receiveBuffer.begin(), this->receiveBuffer.begin() + sizeof(unsigned) + bytesToReceive );
				}
				else
				{
					break;
				}
			}
			else
			{
				this->ErrorClose();
				break;
			}
		}
	}
	
	void FixedConnection::Receive()
	{
		if( this->IsValid() )
		{
			boost::system::error_code ec;
			unsigned long long availableBytesToReceive = this->GetUnreceivedBytes();
			if( availableBytesToReceive > 0 )
			{
				unsigned long long previousBufferSize = this->receiveBuffer.size();
				this->receiveBuffer.resize( previousBufferSize + availableBytesToReceive );
				unsigned long long received = this->socket.read_some( boost::asio::buffer( &(this->receiveBuffer[previousBufferSize]), availableBytesToReceive ), ec );
				this->receiveBuffer.resize( previousBufferSize + received );
				printf( "\n Received: %7llu / %7llu ", received, availableBytesToReceive );
			}
			else
			{
				//printf( "\n No bytes available" );
			}
			
			this->UpdateReceiveBuffer();
		}
		else
		{
			ICon::Error::Push( ICon::Error::Code::tryingToReceiveFromInvalidConnection );
		}
	}
	
	void FixedConnection::ReceiveLock()
	{
		if( this->IsValid() )
		{
			unsigned firstNumberOfMessages = this->buffers.size();
			
			boost::system::error_code ec;
			unsigned long long availableBytesToReceive = 4096;
			unsigned long long previousBufferSize = this->receiveBuffer.size();
			this->receiveBuffer.resize( previousBufferSize + availableBytesToReceive );
			unsigned long long received = this->socket.read_some( boost::asio::buffer( &(this->receiveBuffer[previousBufferSize]), availableBytesToReceive ), ec );
			this->receiveBuffer.resize( previousBufferSize + received );
			this->UpdateReceiveBuffer();
			if( !ec )
			{
				if( this->buffers.size() <= firstNumberOfMessages )
				{
					this->ReceiveLock();
				}
			}
			else
			{
				ICon::Error::Push( ICon::Error::Code::connectionBrokenWhileReceiving );
				this->ErrorClose();
			}
		}
		else
		{
			ICon::Error::Push( ICon::Error::Code::tryingToReceiveFromInvalidConnection );
		}
	}
	
	unsigned FixedConnection::CountReceivedMessages() const
	{
		return this->buffers.size();
	}
	
	const std::vector < unsigned char > & FixedConnection::GetMessageLock()
	{
		if( this->buffers.size() > 0 )
		{
			return this->buffers.front();
		}
		else if( this->IsValid() )
		{
			this->ReceiveLock();
			if( this->buffers.size() > 0 )
			{
				return this->buffers.front();
			}
		}
		ICon::Error::Push( ICon::Error::tryingToGetBufferFromInvalidSocket );
		return this->constReferenceBuffer;
	}
	
	const std::vector < unsigned char > & FixedConnection::GetMessage()
	{
		if( this->buffers.size() > 0 )
		{
			return this->buffers.front();
		}
		ICon::Error::Push( ICon::Error::tryingToAccessUnexistingBuffer );
		return this->constReferenceBuffer;
	}
	
	void FixedConnection::PopMessage( unsigned count )
	{
		if( this->buffers.size() > count )
		{
			this->buffers.erase( this->buffers.begin() );
		}
		else
		{
			this->buffers.clear();
			ICon::Error::Push( ICon::Error::tryingToPopMoreBuffersThanExist );
		}
	}
	
	unsigned long long FixedConnection::Send( const void * buffer, const unsigned bytes )
	{
		if( this->IsValid() )
		{
			if( buffer != nullptr )
			{
				if( bytes > 0 && bytes < FixedConnection::maxBufferSize )
				{
					boost::system::error_code ec;
					unsigned long long ret;
					ret = this->socket.write_some( boost::asio::buffer( &bytes, sizeof(unsigned) ), ec );
					if( ec || ret != sizeof(unsigned) )
					{
						ICon::Error::Push( ICon::Error::connectionClosedByErrorWhileSendingData );
						this->ErrorClose();
						return 0;
					}
					ret = this->socket.write_some( boost::asio::buffer( buffer, bytes ), ec );
					if( ec || ret != bytes )
					{
						ICon::Error::Push( ICon::Error::connectionClosedByErrorWhileSendingData );
						this->ErrorClose();
					}
					return ret;
				}
				else
					ICon::Error::Push( ICon::Error::tryingToSendInvalidDataSize );
			}
			else
				ICon::Error::Push( ICon::Error::tryingToSendInvalidDataPointer );
		}
		else
			ICon::Error::Push( ICon::Error::tryingToSendDataByInvalidConnectino );
		return 0;
	}
	
	void FixedConnection::ErrorClose()
	{
		if( this->IsValid() )
		{
			this->isValid = false;
			this->socket.close();
		}
	}
	
	void FixedConnection::Close()
	{
		if( this->IsValid() )
		{
			boost::system::error_code ec;
			unsigned zero = 0;
			this->socket.write_some( boost::asio::buffer( &zero, sizeof(unsigned) ), ec );
			this->socket.close();
			while( this->isValid )
				this->ReceiveLock();
			this->isValid = false;
		}
	}
	
	FixedConnection::FixedConnection() :
			socket(*ICon::ioService)
	{
		this->isValid = false;
	}
	
	FixedConnection::~FixedConnection()
	{
		this->Close();
	}
};

#endif

