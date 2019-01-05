
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef GLOBAL_BOOST_ASIO_H
#define GLOBAL_BOOST_ASIO_H

#include <boost/asio/io_service.hpp>

namespace ICon
{
	extern class boost::asio::io_context * ioService;
	
	unsigned Init();
	void DeInit();
};

#endif

