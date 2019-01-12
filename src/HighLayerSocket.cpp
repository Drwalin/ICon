
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef HIGH_LAYER_SOCKET_CPP
#define HIGH_LAYER_SOCKET_CPP

#include <boost/asio/basic_stream_socket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/io_service.hpp>

#include "GlobalBoostAsio.h"
#include "HighLayerSocket.h"
#include "Error.h"

#include <chrono>
#include <thread>

namespace ICon
{
	const LoopBuffer & HighLayerSocket::AccessBuffer() const
	{
		return this->buffer;
	}
	
	unsigned HighLayerSocket::GetUnreceivedBytes()
	{
		if( this->IsValid() )
		{
			return ((boost::asio::socket_base::bytes_readable*)(this->command))->get();
		}
		return 0;
	}
	
	bool HighLayerSocket::IsValid() const
	{
		if( this->socket == nullptr )
			return false;
		if( this->command == nullptr )
			return false;
		return this->isValid;
	}
	
	unsigned HighLayerSocket::Connect( const char * ip, const unsigned short port )
	{
		boost::system::error_code ec;
		((boost::asio::ip::tcp::socket*)(this->socket))->connect( boost::asio::ip::tcp::endpoint( boost::asio::ip::address::from_string(ip), port ), ec );
		if( ec )
		{
			ICon::Error::Push( ICon::Error::Code::failedToConnect, __LINE__, __FILE__ );
			return ICon::Error::Code::failedToConnect;
		}
		this->isValid = true;
		((boost::asio::ip::tcp::socket*)(this->socket))->io_control( *((boost::asio::socket_base::bytes_readable*)(this->command)) );
		return ICon::Error::none;
	}
	
	void HighLayerSocket::Receive( bool lockForAnyBytes )
	{
		if( this->IsValid() )
		{
			while( true )
			{
				unsigned long long availableSpaceAtOnce = this->buffer.GetAvailableLengthAtOnceOnEnd();
				unsigned long long availableBytesToReceive = lockForAnyBytes ? availableSpaceAtOnce : this->GetUnreceivedBytes();
				unsigned long long read = availableBytesToReceive < availableSpaceAtOnce ? availableBytesToReceive : availableSpaceAtOnce;
				if( read > 0 )
				{
					boost::system::error_code ec;
					unsigned long long received = ((boost::asio::ip::tcp::socket*)(this->socket))->read_some( boost::asio::buffer( this->buffer.GetEndPtr(), read ), ec );
					this->buffer.PushData( received );
					if( received == 0 )
						ICon::Error::Push( ICon::Error::Code::receivedZeroBytesWhileReceivingNoLock, __LINE__, __FILE__ );
					if( ec )
					{
						ICon::Error::Push( ICon::Error::Code::failedToReceiveBuffer, __LINE__, __FILE__ );
						this->ErrorClose();
						return;
					}
				}
				else
					break;
				lockForAnyBytes = false;
			}
		}
		else
		{
			ICon::Error::Push( ICon::Error::Code::tryingToReceiveFromInvalidHighLayerSocket, __LINE__, __FILE__ );
		}
		
		if( this->buffer.GetBytesStored() >= sizeof(unsigned) )
		{
			unsigned recv = 0;
			this->buffer.GetData( &recv, sizeof(unsigned), 0 );
			if( recv == 0 )
				this->ErrorClose();
		}
	}
	
	void HighLayerSocket::ReceiveClose()
	{
		if( this->IsValid() )
		{
			while( this->IsValid() )
			{
				unsigned messageSize = 0;
				if( this->buffer.GetBytesStored() >= sizeof(unsigned) )
				{
					this->buffer.GetData( &messageSize, sizeof(unsigned), 0 );
					this->buffer.MoveOrigin( sizeof(unsigned) );
					if( recv == 0 )
						this->ErrorClose();
					else
					{
						if( this->buffer.GetBytesStored() > messageSize )
							this->buffer.MoveOrigin( messageSize );
						else
						{
							messageSize -= this->buffer.GetBytesStored();
							this->buffer.Clear();
							if( messageSize != 0 )
							{
								while( messageSize != 0 )
								{
									unsigned long long available = this->buffer.GetAvailableLengthAtOnceOnEnd();
									if( messageSize < available )
										available = messageSize;
									boost::system::error_code ec;
									unsigned long long received = ((boost::asio::ip::tcp::socket*)(this->socket))->read_some( boost::asio::buffer( this->buffer.GetEndPtr(), available ), ec );
									messageSize -= received;
								}
							}
						}
					}
				}
				else
					this->Receive( true );
			}
		}
		else
			ICon::Error::Push( ICon::Error::Code::tryingToReceiveFromInvalidHighLayerSocket, __LINE__, __FILE__ );
	}
	
	bool HighLayerSocket::IsAllMessageAvailable()
	{
		if( this->buffer.GetBytesStored() >= sizeof(unsigned) )
		{
			unsigned recv = 0;
			this->buffer.GetData( &recv, sizeof(unsigned), 0 );
			if( recv == 0 )
				this->ErrorClose();
			else if( this->buffer.GetBytesStored() >= recv + sizeof(unsigned) )
				return true;
		}
		return false;
	}
	
	bool HighLayerSocket::IsMessageAvailable()
	{
		if( this->buffer.GetBytesStored() >= sizeof(unsigned) )
		{
			unsigned recv = 0;
			this->buffer.GetData( &recv, sizeof(unsigned), 0 );
			if( recv == 0 )
				this->ErrorClose();
			else
				return true;
		}
		return false;
	}
	
	unsigned long long HighLayerSocket::GetNextMessageLengthLock()
	{
		if( this->IsValid() )
		{
			while( this->IsValid() && this->buffer.GetBytesStored() < sizeof(unsigned) )
				this->Receive( true );
			if( this->IsValid() )
				return this->GetNextMessageLength();
			else
				ICon::Error::Push( ICon::Error::Code::connectionBrokenWhileReceiving, __LINE__, __FILE__ );
		}
		else
			ICon::Error::Push( ICon::Error::Code::tryingToReceiveFromInvalidHighLayerSocket, __LINE__, __FILE__ );
		return 0;
	}
	
	unsigned long long HighLayerSocket::GetNextMessageLength() const
	{
		if( this->buffer.GetBytesStored() >= sizeof(unsigned) )
		{
			unsigned recv = 0;
			this->buffer.GetData( &recv, sizeof(unsigned), 0 );
			return recv;
		}
		return 0;
	}
	
	unsigned long long HighLayerSocket::GetMessage( void * dst, unsigned long long length, unsigned long long offset )
	{
		if( this->IsAllMessageAvailable() )
		{
			unsigned long long size = this->GetNextMessageLength();
			if( offset + length <= size )
				this->buffer.GetData( dst, length, sizeof(unsigned) + offset );
			else if( offset < size )
				this->buffer.GetData( dst, size-offset, sizeof(unsigned) + offset );
			else
				return 0;
			return size;
		}
		return 0;
	}
	
	unsigned long long HighLayerSocket::GetMessageLock( void * dst, unsigned long long length, unsigned long long offset )
	{
		if( this->IsValid() )
		{
			while( this->IsValid() && this->IsAllMessageAvailable() == false && this->buffer.GetAvailableFreeBytes() != 0 )
				this->Receive( true );
			if( this->IsAllMessageAvailable() )
				return this->GetMessage( dst, length, offset );
		}
		else
			ICon::Error::Push( ICon::Error::Code::tryingToReceiveFromInvalidHighLayerSocket, __LINE__, __FILE__ );
		return 0;
	}
	
	void HighLayerSocket::PopMessage( void * dst )
	{
		if( this->IsAllMessageAvailable() )
		{
			unsigned long long messageSize = this->GetNextMessageLength();
			this->buffer.MoveOrigin( sizeof(unsigned) + messageSize );
		}
		else
			ICon::Error::Push( ICon::Error::Code::tryingToPopEmptyMessageStack, __LINE__, __FILE__ );
	}
	
	void HighLayerSocket::GetPopMessageLock( void * dst )
	{
		if( this->IsValid() || this->IsAllMessageAvailable() )
		{
			while( this->IsValid() && this->buffer.GetBytesStored() < sizeof(unsigned) )
				this->Receive( true );
			
			if( this->IsValid() )
			{
				unsigned messageSize = this->GetNextMessageLength();
				this->buffer.MoveOrigin( sizeof(unsigned) );
				if( this->buffer.GetBytesStored() >= messageSize )
				{
					this->buffer.GetData( dst, messageSize );
					this->buffer.MoveOrigin( messageSize );
				}
				else
				{
					boost::system::error_code ec;
					unsigned current = this->buffer.GetBytesStored();
					this->buffer.GetData( dst, current );
					messageSize -= current;
					dst = ((unsigned char*)(dst)) + current;
					this->buffer.Clear();
					while( messageSize != 0 )
					{
						unsigned long long received = ((boost::asio::ip::tcp::socket*)(this->socket))->read_some( boost::asio::buffer( dst, messageSize ), ec );
						if( ec || received == 0 )
						{
							ICon::Error::Push( ICon::Error::Code::failedToGetPopMessageLockDueToBoostAsioError, __LINE__, __FILE__ );
							this->ErrorClose();
							return;
						}
						messageSize -= received;
						dst = ((unsigned char*)(dst)) + received;
					}
				}
			}
			else
				ICon::Error::Push( ICon::Error::Code::connectionBrokenWhileReceiving, __LINE__, __FILE__ );
		}
		else
			ICon::Error::Push( ICon::Error::Code::tryingToReceiveFromInvalidHighLayerSocket, __LINE__, __FILE__ );
	}
	
	unsigned long long HighLayerSocket::Send( const void * buffer, const unsigned bytes )
	{
		if( this->IsValid() )
		{
			if( buffer != nullptr )
			{
				if( bytes > 0 && bytes < HighLayerSocket::maxMessageSize )
				{
					boost::system::error_code ec;
					unsigned long long ret;
					ret = ((boost::asio::ip::tcp::socket*)(this->socket))->write_some( boost::asio::buffer( &bytes, sizeof(unsigned) ), ec );
					if( ec || ret != sizeof(unsigned) )
					{
						ICon::Error::Push( ICon::Error::connectionClosedByErrorWhileSendingData, __LINE__, __FILE__ );
						this->ErrorClose();
						return 0;
					}
					ret = ((boost::asio::ip::tcp::socket*)(this->socket))->write_some( boost::asio::buffer( buffer, bytes ), ec );
					if( ec || ret != bytes )
					{
						ICon::Error::Push( ICon::Error::connectionClosedByErrorWhileSendingData, __LINE__, __FILE__ );
						this->ErrorClose();
					}
					return ret;
				}
				else
					ICon::Error::Push( ICon::Error::tryingToSendInvalidDataSize, __LINE__, __FILE__ );
			}
			else
				ICon::Error::Push( ICon::Error::tryingToSendInvalidDataPointer, __LINE__, __FILE__ );
		}
		else
			ICon::Error::Push( ICon::Error::tryingToSendDataByInvalidHighSocketLayer, __LINE__, __FILE__ );
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
						ICon::Error::Push( ICon::Error::tryingToSendInvalidBuffersArray, __LINE__, __FILE__ );
						return 0;
					}
					sumBytesL += bytes[i];
				}
				
				if( sumBytesL < buffers || sumBytesL > (unsigned long long)ICon::HighLayerSocket::maxMessageSize )
				{
					ICon::Error::Push( ICon::Error::tryingToSendInvalidDataSize, __LINE__, __FILE__ );
					return 0;
				}
				
				boost::system::error_code ec;
				unsigned sumBytes = sumBytesL;
				
				ret = ((boost::asio::ip::tcp::socket*)(this->socket))->write_some( boost::asio::buffer( &sumBytes, sizeof(unsigned) ), ec );
				if( ec || ret != sizeof(unsigned) )
				{
					ICon::Error::Push( ICon::Error::connectionClosedByErrorWhileSendingData, __LINE__, __FILE__ );
					this->ErrorClose();
					return 0;
				}
				
				sumBytesL = 0;
				ret = 0;
				for( i = 0; i < buffers; ++i )
				{
					sumBytesL += bytes[i];
					ret += ((boost::asio::ip::tcp::socket*)(this->socket))->write_some( boost::asio::buffer( buffer[i], bytes[i] ), ec );
					if( ec || ret != sumBytesL )
					{
						ICon::Error::Push( ICon::Error::connectionClosedByErrorWhileSendingData, __LINE__, __FILE__ );
						this->ErrorClose();
						return ret;
					}
				}
				return ret;
			}
			else
				ICon::Error::Push( ICon::Error::tryingToSendInvalidBuffersArray, __LINE__, __FILE__ );
		}
		else
			ICon::Error::Push( ICon::Error::tryingToSendDataByInvalidHighSocketLayer, __LINE__, __FILE__ );
		return 0;
	}
	
	void HighLayerSocket::ErrorClose()
	{
		if( this->IsValid() )
		{
			this->isValid = false;
			unsigned zero = 0;
			((boost::asio::ip::tcp::socket*)(this->socket))->write_some( boost::asio::buffer( &zero, sizeof(unsigned) ) );
			((boost::asio::ip::tcp::socket*)(this->socket))->close();
			((boost::asio::ip::tcp::socket*)(this->socket))->close();
			((boost::asio::ip::tcp::socket*)(this->socket))->close();
		}
	}
	
	void HighLayerSocket::Close()
	{
		if( this->IsValid() )
		{
			unsigned zero = 0;
			((boost::asio::ip::tcp::socket*)(this->socket))->write_some( boost::asio::buffer( &zero, sizeof(unsigned) ) );
			this->ReceiveClose();
			((boost::asio::ip::tcp::socket*)(this->socket))->close();
			((boost::asio::ip::tcp::socket*)(this->socket))->close();
			((boost::asio::ip::tcp::socket*)(this->socket))->close();
			this->isValid = false;
		}
	}
	
	HighLayerSocket::HighLayerSocket() :
			socket( new boost::asio::ip::tcp::socket( *((boost::asio::io_service*)(ICon::ioService)) ) ),
			command( new boost::asio::socket_base::bytes_readable() )
	{
		this->isValid = false;
		this->buffer.Init( 1024, 1024 );
	}
	
	HighLayerSocket::~HighLayerSocket()
	{
		this->Close();
		if( this->socket )
			delete ((boost::asio::ip::tcp::socket*)(this->socket));
		this->socket = nullptr;
		if( this->command )
			delete ((boost::asio::socket_base::bytes_readable*)(this->command));
		this->command = nullptr;
	}
};

#endif

