
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef DEEP_SERVER_CPP
#define DEEP_SERVER_CPP

#include "DeepServer.h"
#include "Error.h"

#include <chrono>
#include <thread>

namespace ICon
{
	unsigned DeepServer::Accept( std::shared_ptr<FixedConnection> con )
	{
		if( this->opened )
		{
			if( con )
			{
				if( this->isAcceptNoLockRunning.load() == false )
				{
					boost::system::error_code ec;
					this->acceptor.accept( con->socket, ec );
					if( !ec )
					{
						con->isValid = true;
						con->socket.io_control( con->command );
						return ICon::Error::none;
					}
					else
					{
						ICon::Error::Push( ICon::Error::Code::failedToAccept );
						return ICon::Error::Code::failedToAccept;
					}
				}
				else
				{
					ICon::Error::Push( ICon::Error::tryingToRunAcceptWhileAcceptNoLockIsRunning );
					return ICon::Error::tryingToRunAcceptWhileAcceptNoLockIsRunning;
				}
			}
			else
			{
				ICon::Error::Push( ICon::Error::tryingToAcceptToUnallocatedConnection );
				return ICon::Error::tryingToAcceptToUnallocatedConnection;
			}
		}
		else
		{
			ICon::Error::Push( ICon::Error::tryingToAcceptUsingClosedServer );
			return ICon::Error::tryingToAcceptUsingClosedServer;
		}
		return ICon::Error::unknown;
	}
	
	std::shared_ptr<DeepServer> DeepServer::Make()
	{
		std::shared_ptr<DeepServer> ret( new DeepServer() );
		ret->self = ret;
		return ret;
	}
	
	unsigned DeepServer::StartListening( const unsigned short port )
	{
		this->port = port;
		this->endpoint = boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port );
		this->acceptor = boost::asio::ip::tcp::acceptor( *ICon::ioService, this->endpoint );
		this->opened = true;
		return ICon::Error::none;
	}
	
	void DeepServer::StopListening()
	{
		if( this->opened )
		{
			this->keepAcceptNoLockRunning.store( false );
			if( this->isAcceptNoLockRunning.load() == true )
			{
				std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
				if( this->isAcceptNoLockRunning.load() == true )
				{
					std::shared_ptr<FixedConnection> closingConnection( new FixedConnection );
					if( closingConnection->Connect( "127.0.0.1", this->port ) == ICon::Error::none )
					{
						closingConnection->Close();
					}
				}
			}
		}
	}
	
	void DeepServer::Close()
	{
		if( this->opened )
		{
			this->StopListening();
			this->acceptor.close();
			//this->acceptor = boost::asio::ip::tcp::acceptor( *ICon::ioService );
			//this->endpoint = boost::asio::ip::tcp::endpoint();
			this->port = 0;
			this->opened = false;
		}
	}
	
	DeepServer::DeepServer() :
		isAcceptNoLockRunning(false), keepAcceptNoLockRunning(false),
		acceptor(*ICon::ioService)
	{
		this->self = nullptr;
		this->port = 0;
		this->opened = false;
	}
	
	DeepServer::~DeepServer()
	{
		this->Close();
	}
};

#endif

