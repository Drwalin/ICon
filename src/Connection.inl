
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
			this->buffer.reserve( 1024*512 );
			unsigned long long typeSize = Binary::Type::Get( this->buffer, obj );
			unsigned long long messageSize = Binary::Store( this->buffer, obj, typeSize );
			this->con->Send( &(this->buffer.front()), messageSize );
		}
		else
		{
			ICon::Error::Push( ICon::Error::Code::noConnection );
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
				this->buffer.reserve( 1024*512 );
				unsigned long long typeSize = Binary::Type::Get( this->buffer, obj );
				
				std::vector < unsigned char > & receivedMessage = this->con->GetMessageLock();
				
				if( this->con->IsReferencingToNothing( receivedMessage ) )
				{
					if( Binary::Type::IsValid( this->buffer, obj ) )
					{
						unsigned long long resotred = Binary::Restore( this->buffer, obj, typeSize );
						if( restored == 0 )
							ICon::Error::Push( ICon::Error::code::tryingToReceiveToInvalidType );
					}
					else
						ICon::Error::Push( ICon::Error::code::tryingToReceiveToInvalidType );
				}
				else
					ICon::Error::Push( ICon::Error::Code::failedToGetMessageLockWhichReturnedConstReference );
			}
			else
				ICon::Error::Push( ICon::Error::Code::failedToReceiveLock );
		}
		else
			ICon::Error::Push( ICon::Error::Code::noConnection );
		this->buffer.resize( 0 );
		return *this;
	}
};

#endif

