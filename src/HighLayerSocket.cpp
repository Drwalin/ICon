
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef HIGH_LAYER_SOCKET_CPP
#define HIGH_LAYER_SOCKET_CPP

#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>

#include "GlobalBoostAsio.h"
#include "HighLayerSocket.h"
#include "Error.h"

#include <chrono>
#include <thread>

namespace ICon
{
	bool HighLayerSocket::IsReferencingToNothing( const std::vector < unsigned char > & var ) const
	{
		return &var == &constReferenceBuffer;
	}
	
	unsigned HighLayerSocket::GetUnreceivedBytes()
	{
		if( this->IsValid() )
		{
			return this->command.get();
		}
		return 0;
	}
	
	bool HighLayerSocket::IsValid() const
	{
		return this->isValid;
	}
	
	unsigned HighLayerSocket::Connect( const char * ip, const unsigned short port )
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
	
	void HighLayerSocket::UpdateReceiveBuffer()
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
	
	void HighLayerSocket::Receive()
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
	
	void HighLayerSocket::ReceiveLock( bool doNotThrowErrorBecauseOfClosingSequence )
	{
		if( this->IsValid() )
		{
			while( this->buffers.size() == 0 )
			{
				boost::system::error_code ec;
				unsigned long long availableBytesToReceive = 4096;
				unsigned long long previousBufferSize = this->receiveBuffer.size();
				this->receiveBuffer.resize( previousBufferSize + availableBytesToReceive );
				unsigned long long received = this->socket.read_some( boost::asio::buffer( &(this->receiveBuffer[previousBufferSize]), availableBytesToReceive ), ec );
				this->receiveBuffer.resize( previousBufferSize + received );
				this->UpdateReceiveBuffer();
				if( ec )
				{
					if( doNotThrowErrorBecauseOfClosingSequence == false )
						ICon::Error::Push( ICon::Error::Code::connectionBrokenWhileReceiving );
					this->ErrorClose();
					return;
				}
			}
		}
		else if( doNotThrowErrorBecauseOfClosingSequence == false )
			ICon::Error::Push( ICon::Error::Code::tryingToReceiveFromInvalidConnection );
	}
	
	unsigned HighLayerSocket::CountReceivedMessages() const
	{
		return this->buffers.size();
	}
	
	const std::vector < unsigned char > & HighLayerSocket::GetMessageLock()
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
	
	const std::vector < unsigned char > & HighLayerSocket::GetMessage()
	{
		if( this->buffers.size() > 0 )
		{
			return this->buffers.front();
		}
		ICon::Error::Push( ICon::Error::tryingToAccessUnexistingBuffer );
		return this->constReferenceBuffer;
	}
	
	void HighLayerSocket::PopMessage( unsigned count )
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
	
	unsigned long long HighLayerSocket::Send( const void * buffer, const unsigned bytes )
	{
		if( this->IsValid() )
		{
			if( buffer != nullptr )
			{
				if( bytes > 0 && bytes < HighLayerSocket::maxBufferSize )
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
	
	unsigned long long HighLayerSocket::Send( const void ** buffer, const unsigned * bytes, const unsigned buffers )
	{
		if( this->IsValid() )
		{
			if( buffer != nullptr && bytes != nullptr && buffers > 0 )
			{
				unsigned long long sumBytesL = 0, ret;
				unsigned i;
				for( i = 0; i < buffers; ++i )
				{
					if( buffer[i] == nullptr || bytes[i] == 0 )
					{
						ICon::Error::Push( ICon::Error::tryingToSendInvalidBuffersArray );
						return 0;
					}
					sumBytesL += bytes[i];
				}
				
				if( sumBytesL < buffers || sumBytesL > (unsigned long long)ICon::HighLayerSocket::maxBufferSize )
				{
					ICon::Error::Push( ICon::Error::tryingToSendInvalidDataSize );
					return 0;
				}
				
				boost::system::error_code ec;
				unsigned sumBytes = sumBytesL;
				
				ret = this->socket.write_some( boost::asio::buffer( &sumBytes, sizeof(unsigned) ), ec );
				if( ec || ret != sizeof(unsigned) )
				{
					ICon::Error::Push( ICon::Error::connectionClosedByErrorWhileSendingData );
					this->ErrorClose();
					return 0;
				}
				
				sumBytesL = 0;
				ret = 0;
				for( i = 0; i < buffers; ++i )
				{
					sumBytesL += bytes[i];
					ret += this->socket.write_some( boost::asio::buffer( buffer[i], bytes[i] ), ec );
					if( ec || ret != sumBytesL )
					{
						ICon::Error::Push( ICon::Error::connectionClosedByErrorWhileSendingData );
						this->ErrorClose();
						return ret;
					}
				}
				return ret;
			}
			else
				ICon::Error::Push( ICon::Error::tryingToSendInvalidBuffersArray );
		}
		else
			ICon::Error::Push( ICon::Error::tryingToSendDataByInvalidConnectino );
		return 0;
	}
	
	void HighLayerSocket::ErrorClose()
	{
		if( this->IsValid() )
		{
			this->isValid = false;
			unsigned zero = 0;
			this->socket.write_some( boost::asio::buffer( &zero, sizeof(unsigned) ) );
			this->socket.close();
			this->socket.close();
			this->socket.close();
		}
	}
	
	void HighLayerSocket::Close()
	{
		if( this->IsValid() )
		{
			unsigned zero = 0;
			this->socket.write_some( boost::asio::buffer( &zero, sizeof(unsigned) ) );
			while( this->IsValid() )
			{
				this->buffers.resize( 0 );
				this->ReceiveLock( true );
			}
			this->socket.close();
			this->socket.close();
			this->socket.close();
			this->isValid = false;
		}
	}
	
	HighLayerSocket::HighLayerSocket() :
			socket(*ICon::ioService)
	{
		this->isValid = false;
	}
	
	HighLayerSocket::~HighLayerSocket()
	{
		this->Close();
	}
};

#endif
