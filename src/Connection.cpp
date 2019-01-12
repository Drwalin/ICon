
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
		if( this->con != nullptr )
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
			return this->con->IsMessageAvailable();
		}
		return false;
	}
	
	void Connection::WaitForAnythingToReceive()
	{
		if( this->IsValid() )
		{
			while( this->con->IsValid() && this->con->IsAllMessageAvailable() == false && this->con->AccessBuffer().GetAvailableFreeBytes() != 0 )
				this->con->Receive( true );
		}
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

