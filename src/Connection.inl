
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef CONNECTION_INL
#define CONNECTION_INL

#include "BinaryType.h"

namespace ICon
{
	template < typename T >
	Connection & Connection::operator << ( const T & obj )
	{
		if( this->IsValid() )
		{
			this->buffer.clear();
			this->buffer.reserve( 1024*512 );
			unsigned long long typeSize = Binary::Type::Get( this->buffer, obj );
			printf( "\n send::type: " );
			Print( stderr, this->buffer );
			unsigned long long messageSize = Binary::Store( this->buffer, obj, typeSize );
			printf( "\n send::message (%llu): ", messageSize );
			Print( stderr, this->buffer );
			this->con->Send( &(this->buffer.front()), messageSize );
		}
		else
		{
			ICon::Error::Push( ICon::Error::Code::tryingToSendThorughInvalidConnection, __LINE__, __FILE__ );
		}
		this->buffer.resize( 0 );
		return *this;
	}
	
	template < typename T >
	Connection & Connection::operator >> ( T & obj )
	{
		if( this->IsValid() )
		{
			this->con->ReceiveLock();
			
			if( this->con->CountReceivedMessages() > 0 )
			{
				this->buffer.clear();
				this->buffer.reserve( 1024*512 );
				const unsigned long long typeSize = Binary::Type::Get( this->buffer, obj );
				printf( "\n recv::type: " );
				Print( stderr, this->buffer );
				
				const std::vector < unsigned char > & receivedMessage = this->con->GetMessageLock();
				printf( "\n recv::message: " );
				Print( stderr, receivedMessage );
				
				if( receivedMessage.size() != 0 )
				{
					if( Binary::Type::IsValid( receivedMessage, obj, 0 ) )
					{
						unsigned long long restored = Binary::Restore( receivedMessage, obj, typeSize );
						if( restored == 0 )
							ICon::Error::Push( ICon::Error::Code::failedToReceiveToValidType, __LINE__, __FILE__ );
						this->con->PopMessage();
					}
					else
						ICon::Error::Push( ICon::Error::Code::tryingToReceiveToInvalidType, __LINE__, __FILE__ );
				}
				else
					ICon::Error::Push( ICon::Error::Code::failedToGetMessageLockWhichReturnedConstReference, __LINE__, __FILE__ );
			}
			else
				ICon::Error::Push( ICon::Error::Code::failedToReceiveLock, __LINE__, __FILE__ );
		}
		else
			ICon::Error::Push( ICon::Error::Code::tryingToReceiveFromInvalidConnection, __LINE__, __FILE__ );
		this->buffer.resize( 0 );
		return *this;
	}
};

#endif

