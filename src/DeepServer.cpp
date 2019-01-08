
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef DEEP_SERVER_CPP
#define DEEP_SERVER_CPP

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "DeepServer.h"
#include "Error.h"

#include <chrono>
#include <thread>

namespace ICon
{
	bool DeepServer::IsOpened()
	{
		if( this->acceptor == nullptr )
			return false;
		if( this->endpoint == nullptr )
			return false;
		return this->opened;
	}
	
	unsigned DeepServer::Accept( std::shared_ptr<HighLayerSocket> con )
	{
		if( this->IsOpened() )
		{
			if( con )
			{
				if( this->isAcceptNoLockRunning.load() == false )
				{
					boost::system::error_code ec;
					((boost::asio::ip::tcp::acceptor*)(this->acceptor))->accept( *((boost::asio::ip::tcp::socket*)(con->socket)), ec );
					if( !ec )
					{
						con->isValid = true;
						((boost::asio::ip::tcp::socket*)(con->socket))->io_control( *((boost::asio::socket_base::bytes_readable*)(con->command)) );
						return ICon::Error::none;
					}
					else
					{
						ICon::Error::Push( ICon::Error::Code::failedToAccept, __LINE__, __FILE__ );
						return ICon::Error::Code::failedToAccept;
					}
				}
				else
				{
					ICon::Error::Push( ICon::Error::tryingToRunAcceptWhileAcceptNoLockIsRunning, __LINE__, __FILE__ );
					return ICon::Error::tryingToRunAcceptWhileAcceptNoLockIsRunning;
				}
			}
			else
			{
				ICon::Error::Push( ICon::Error::tryingToAcceptToUnallocatedHighSocketLayer, __LINE__, __FILE__ );
				return ICon::Error::tryingToAcceptToUnallocatedHighSocketLayer;
			}
		}
		else
		{
			ICon::Error::Push( ICon::Error::tryingToAcceptUsingClosedServer, __LINE__, __FILE__ );
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
		this->Close();
		this->port = port;
		this->endpoint = new boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port );
		this->acceptor = new boost::asio::ip::tcp::acceptor( *((boost::asio::io_service*)(ICon::ioService)), *((boost::asio::ip::tcp::endpoint*)(this->endpoint)) );
		if( this->endpoint && this->acceptor )
		{
			this->opened = true;
			return ICon::Error::none;
		}
		ICon::Error::Push( ICon::Error::Code::failedToAllocateBoostAsioEndpointOrAcceptor, __LINE__, __FILE__ );
		return ICon::Error::Code::failedToAllocateBoostAsioEndpointOrAcceptor;
	}
	
	void DeepServer::StopListening()
	{
		if( this->IsOpened() )
		{
			this->keepAcceptNoLockRunning.store( false );
			if( this->isAcceptNoLockRunning.load() == true )
			{
				std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
				if( this->isAcceptNoLockRunning.load() == true )
				{
					std::shared_ptr<HighLayerSocket> closingConnection( new HighLayerSocket );
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
		if( this->IsOpened() )
		{
			this->StopListening();
			((boost::asio::ip::tcp::acceptor*)(this->acceptor))->close();
			this->opened = false;
		}
		this->port = 0;
		if( this->endpoint )
			delete ((boost::asio::ip::tcp::endpoint*)(this->endpoint));
		this->endpoint = nullptr;
		if( this->acceptor )
			delete ((boost::asio::ip::tcp::acceptor*)(this->acceptor));
		this->acceptor = nullptr;
	}
	
	DeepServer::DeepServer() :
		isAcceptNoLockRunning(false), keepAcceptNoLockRunning(false)
	{
		this->self = nullptr;
		this->port = 0;
		this->opened = false;
		this->acceptor = nullptr;
		this->endpoint = nullptr;
	}
	
	DeepServer::~DeepServer()
	{
		this->Close();
	}
};

#endif

