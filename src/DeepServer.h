
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef DEEP_SERVER_H
#define DEEP_SERVER_H

#include <memory>
#include <atomic>
#include <thread>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "HighLayerSocket.h"
#include "Error.h"

namespace ICon
{
	class DeepServer final
	{
	private:
		
		unsigned short port;
		bool opened;
		
		boost::asio::ip::tcp::endpoint endpoint;
		boost::asio::ip::tcp::acceptor acceptor;
		
		std::atomic<bool> isAcceptNoLockRunning;
		std::atomic<bool> keepAcceptNoLockRunning;
		
		std::shared_ptr<DeepServer> self;
		
		template < typename CallBackType, typename... Args >
		friend void _DeepServer_AcceptNoLock_ThreadFunction_OneAccept( std::shared_ptr<DeepServer> server, std::shared_ptr<HighLayerSocket> con, CallBackType callBackFunction, Args... args );;
		
		template < typename CallBackType, typename... Args >
		friend void _DeepServer_AcceptNoLock_ThreadFunction_OneAccept( std::shared_ptr<DeepServer> server, CallBackType callBackFunction, Args... args );
		
		template < typename CallBackType, typename... Args >
		void AcceptNoLockRunned( std::shared_ptr<HighLayerSocket> con, CallBackType callBackFunction, Args... args );
		
		template < typename CallBackType, typename... Args >
		void AcceptNoLockRunned( CallBackType callBackFunction, Args... args );
		
		DeepServer();
		
	public:
		
		unsigned Accept( std::shared_ptr<HighLayerSocket> con );
		
		// only one istance of AcceptNoLock can be running at one time
		template < typename CallBackType, typename... Args >
		void AcceptNoLock( std::shared_ptr<HighLayerSocket> con, CallBackType callBackFunction, Args... args );
		
		template < typename CallBackType, typename... Args >
		void AcceptNoLock( CallBackType callBackFunction, Args... args );
		
		
		static std::shared_ptr<DeepServer> Make();
		
		unsigned StartListening( const unsigned short port );
		void StopListening();
		
		void Close();
		
		~DeepServer();
	};
	
	template < typename CallBackType, typename... Args >
	void _DeepServer_AcceptNoLock_ThreadFunction_OneAccept( std::shared_ptr<DeepServer> server, std::shared_ptr<HighLayerSocket> con, CallBackType callBackFunction, Args... args );
	
	template < typename CallBackType, typename... Args >
	void _DeepServer_AcceptNoLock_ThreadFunction_OneAccept( std::shared_ptr<DeepServer> server, CallBackType callBackFunction, Args... args );
};

#include "DeepServer.inl"

#endif

