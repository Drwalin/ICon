
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef CONNECTION_CPP
#define CONNECTION_CPP

#include "Connection.h"

namespace ICon
{
	bool Connection::IsValid()
	{
		if( this->con )
		{
			if( this->con->IsValid() )
			{
				return true;
			}
		}
		return false;
	}
	
	void Connection::UpdateReceive()
	{
		if( this->IsValid() )
		{
			this->con->Receive();
		}
	}
	
	bool Connection::IsAnythingToRecevie( bool updateReceive )
	{
		if( this->IsValid() && updateReceive )
		{
			this->UpdateReceive();
		}
		if( this->con )
		{
			return this->con->CountReceivedMessages() != 0;
		}
		return false;
	}
	
	void Connection::WaitForAnythingToReceive()
	{
		if( this->IsValid() )
		{
			this->con->ReceiveLock();
		}
	}
	
	unsigned long long Connection::write( const void * src, const unsigned size )
	{
		if( this->IsValid() )
		{
			this->buffer.resize( 1 );
			this->buffer[0] = Binary::Type::RAW;
			
			const void * buffers[2] = { &(this->buffer.front()), src };
			const unsigned bytes[2] = { 1, size };
			
			unsigned long long ret = this->con->Send( buffers, bytes, 2 );
			this->buffer.resize( 0 );
			return ret;
		}
		else
		{
			ICon::Error::Push( ICon::Error::Code::tryingToSendThroughInvalidConnection );
		}
		this->buffer.resize( 0 );
		return 0;
	}
	
	unsigned long long Connection::read( void * dst, const unsigned size )
	{
		if( this->IsValid() )
		{
			if( this->buffer.size() > 0 )
			{
				if( this->buffer.size() <= size )
				{
					memmove( dst, &(this->buffer.front()), this->buffer.size() );
					unsigned long long ret =this->buffer.size();
					this->buffer.resize( 0 );
					return ret;
				}
				else
				{
					memmove( dst, &(this->buffer.front()), size );
					this->buffer.erase( this->buffer.begin(), this->buffer.begin() + size );
					return size;
				}
			}
			else
			{
				if( this->buffer[0] == Binary::Type::RAW )
				{
					this->buffer = this->con->GetMessageLock();
					return this->read( dst, size );
				}
				else
					ICon::Error::Push( ICon::Error::Code::tryingToReadFromBufferOfTypeRaw );
			}	
		}
		else
			ICon::Error::Push( ICon::Error::Code::tryingToReadFromInvalidConnection );
		return 0;
	}
	
	unsigned long long Connection::GetNextBufferSize()
	{
		if( this->buffer.size() > 0 )
		{
			return this->buffer.size();
		}
		if( this->IsValid() )
		{
			unsigned long long messageSize = this->con->GetMessageLock().size();
			if( messageSize > 0 )
			{
				unsigned long long typeSize = Binary::Type::GetOffset( this->con->GetMessageLock() );
				if( typeSize > messageSize )
				{
					return messageSize - typeSize;
				}
				else
					ICon::Error::Push( ICon::Error::Code::receivedInvalidBuffer );
			}
			else
				ICon::Error::Push( ICon::Error::Code::failedToReceiveBuffer );
		}
		else
			ICon::Error::Push( ICon::Error::Code::tryingToReadFromInvalidConnection );
		return 0;
	}
	
	void Connection::SetHighLayerSocket( std::shared_ptr<HighLayerSocket> con )
	{
		this->con = con;
	}
	
	std::shared_ptr<HighLayerSocket> Connection::GetHighLayerSocket()
	{
		return this->con;
	}
	
	void Connection::Close()
	{
		if( this->IsValid() )
		{
			this->con->Close();
		}
	}
	
	Connection::Connection( std::shared_ptr<HighLayerSocket> con_ ) :
		con(con_)
	{
		this->buffer.reserve( 1024*512 );
	}
	
	Connection::Connection() :
		con(new HighLayerSocket())
	{
		this->buffer.reserve( 1024*512 );
	}
	
	Connection::~Connection()
	{
	}
};

#endif

