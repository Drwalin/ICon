
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef GLOBAL_BOOST_ASIO_CPP
#define GLOBAL_BOOST_ASIO_CPP

#include <boost/asio/io_service.hpp>

#include "GlobalBoostAsio.h"
#include "Error.h"

namespace ICon
{
	/*boost::asio::io_service*/ void * ioService = nullptr;
	
	unsigned Init()
	{
		if( ICon::ioService == nullptr )
		{
			ICon::ioService = new boost::asio::io_service;
			if( ICon::ioService )
			{
				((boost::asio::io_service*)(ICon::ioService))->run();
				return 0;
			}
		}
		else
		{
			return true;
		}
		ICon::Error::Push( ICon::Error::Code::failedToInitIoService, __LINE__, __FILE__ );
		return false;
	}
	
	void DeInit()
	{
		if( ICon::ioService != nullptr )
		{
			delete ((boost::asio::io_service*)(ICon::ioService));
		}
	}
};

#endif

