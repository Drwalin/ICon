
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <atomic>
#include <thread>

#include "HighLayerSocket.h"
#include "Error.h"

namespace ICon
{
	class Server final
	{
	private:
		
		unsigned short port;
		bool opened;
		
		/*boost::asio::ip::tcp::endpoint*/ void * endpoint;
		/*boost::asio::ip::tcp::acceptor*/ void * acceptor;
		
		std::atomic<bool> isAcceptNoLockRunning;
		std::atomic<bool> keepAcceptNoLockRunning;
		
		std::shared_ptr<Server> self;
		
		template < typename CallBackType, typename... Args >
		friend void _Server_AcceptNoLock_ThreadFunction_OneAccept( std::shared_ptr<Server> server, std::shared_ptr<HighLayerSocket> con, CallBackType callBackFunction, Args... args );;
		
		template < typename CallBackType, typename... Args >
		friend void _Server_AcceptNoLock_ThreadFunction_OneAccept( std::shared_ptr<Server> server, CallBackType callBackFunction, Args... args );
		
		template < typename CallBackType, typename... Args >
		void AcceptNoLockRunned( std::shared_ptr<HighLayerSocket> con, CallBackType callBackFunction, Args... args );
		
		template < typename CallBackType, typename... Args >
		void AcceptNoLockRunned( CallBackType callBackFunction, Args... args );
		
		Server();
		
	public:
		
		bool IsOpened();
		
		unsigned Accept( std::shared_ptr<HighLayerSocket> con );
		
		// only one istance of AcceptNoLock can be running at one time
		template < typename CallBackType, typename... Args >
		void AcceptNoLock( std::shared_ptr<HighLayerSocket> con, CallBackType callBackFunction, Args... args );
		
		template < typename CallBackType, typename... Args >
		void AcceptNoLock( CallBackType callBackFunction, Args... args );
		
		
		static std::shared_ptr<Server> Make();
		
		unsigned StartListening( const unsigned short port );
		void StopListening();
		
		void Close();
		
		~Server();
	};
	
	template < typename CallBackType, typename... Args >
	void _Server_AcceptNoLock_ThreadFunction_OneAccept( std::shared_ptr<Server> server, std::shared_ptr<HighLayerSocket> con, CallBackType callBackFunction, Args... args );
	
	template < typename CallBackType, typename... Args >
	void _Server_AcceptNoLock_ThreadFunction_OneAccept( std::shared_ptr<Server> server, CallBackType callBackFunction, Args... args );
};

#include "Server.inl"

#endif

