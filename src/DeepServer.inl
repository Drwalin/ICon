
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef DEEP_SERVER_INL
#define DEEP_SERVER_INL

namespace ICon
{
	template < typename CallBackType, typename... Args >
	void _DeepServer_AcceptNoLock_ThreadFunction_OneAccept( std::shared_ptr<DeepServer> server, std::shared_ptr<FixedConnection> con, CallBackType callBackFunction, Args... args )
	{
		server->AcceptNoLockRunned( con, callBackFunction, args... );
	}
	
	template < typename CallBackType, typename... Args >
	void _DeepServer_AcceptNoLock_ThreadFunction_OneAccept( std::shared_ptr<DeepServer> server, CallBackType callBackFunction, Args... args )
	{
		server->AcceptNoLockRunned( callBackFunction, args... );
	}
	
	
	template < typename CallBackType, typename... Args >
	void DeepServer::AcceptNoLockRunned( std::shared_ptr<FixedConnection> con, CallBackType callBackFunction, Args... args )
	{
		while( this->Accept( con ) != ICon::Error::none ){}
		this->isAcceptNoLockRunning.store( false );
		callBackFunction( con, args... );
	}
	
	template < typename CallBackType, typename... Args >
	void DeepServer::AcceptNoLockRunned( CallBackType callBackFunction, Args... args )
	{
		while( this->keepAcceptNoLockRunning.load() == true )
		{
			std::shared_ptr<FixedConnection> con( new FixedConnection );
			if( this->Accept( con ) == ICon::Error::none )
			{
				if( con->IsValid() )
				{
					std::thread( callBackFunction, con, args... ).detach();
				}
			}
		}
		this->isAcceptNoLockRunning.store( false );
	}
	
	
	template < typename CallBackType, typename... Args >
	void DeepServer::AcceptNoLock( std::shared_ptr<FixedConnection> con, CallBackType callBackFunction, Args... args )
	{
		if( this->opened )
		{
			if( this->isAcceptNoLockRunning.load() == false )
			{
				if( con )
				{
					this->isAcceptNoLockRunning.store( true );
					std::thread( _DeepServer_AcceptNoLock_ThreadFunction_OneAccept, self, con, callBackFunction, args... ).detach();
				}
				else
				{
					ICon::Error::Push( ICon::Error::tryingToAcceptToUnallocatedConnection );
				}
			}
			else
			{
				ICon::Error::Push( ICon::Error::tryingToRunSecondInstancOfAcceptNoLock );
			}
		}
		else
		{
			ICon::Error::Push( ICon::Error::tryingToAcceptUsingClosedServer );
		}
	}
	
	template < typename CallBackType, typename... Args >
	void DeepServer::AcceptNoLock( CallBackType callBackFunction, Args... args )
	{
		if( this->opened )
		{
			if( this->isAcceptNoLockRunning.load() == false )
			{
				this->keepAcceptNoLockRunning.store( true );
				this->isAcceptNoLockRunning.store( true );
				std::thread( _DeepServer_AcceptNoLock_ThreadFunction_OneAccept, self, callBackFunction, args... ).detach();
			}
			else
			{
				ICon::Error::Push( ICon::Error::tryingToRunSecondInstancOfAcceptNoLock );
			}
		}
		else
		{
			ICon::Error::Push( ICon::Error::tryingToAcceptUsingClosedServer );
		}
	}
};

#endif

