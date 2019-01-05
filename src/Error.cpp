
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef ERROR_CPP
#define ERROR_CPP

#include "Error.h"

namespace ICon
{
	std::mutex globalErrorQueueModificationMutex;
	std::vector < Error > globalErrorQueue;
	bool printErrorWhenPushed = false;
	
	
	void Error::PrintWhenPushed()
	{
		ICon::printErrorWhenPushed = true;
	}
	
	void Error::DoNotPrintWhenPushed()
	{
		ICon::printErrorWhenPushed = false;
	}
	
	void Error::Push( unsigned code, const std::string & message, void * ptr )
	{
		ICon::globalErrorQueueModificationMutex.lock();
		ICon::globalErrorQueue.emplace_back( code, message, ptr );
		if( ICon::printErrorWhenPushed )
			ICon::globalErrorQueue.back().Print();
		ICon::globalErrorQueueModificationMutex.unlock();
	}
	
	void Error::Push( unsigned code )
	{
		ICon::globalErrorQueueModificationMutex.lock();
		ICon::globalErrorQueue.emplace_back( code, "", nullptr );
		if( ICon::printErrorWhenPushed )
			ICon::globalErrorQueue.back().Print();
		ICon::globalErrorQueueModificationMutex.unlock();
	}
	
	Error Error::Pop()
	{
		ICon::globalErrorQueueModificationMutex.lock();
		if( ICon::globalErrorQueue.size() )
		{
			Error ret = ICon::globalErrorQueue.front();
			ICon::globalErrorQueue.erase( ICon::globalErrorQueue.begin() );
			ICon::globalErrorQueueModificationMutex.unlock();
			return ret;
		}
		ICon::globalErrorQueueModificationMutex.unlock();
		return Error( ICon::Error::Code::tryingToAccessEmptyErrorQueue, "", nullptr);
	}
	
	Error::operator unsigned() const
	{
		return this->code;
	}
	
	void Error::Print( std::ostream & stream )
	{
		stream << "\n ICon::Error appeard with code [" << this->code << "]: " << ICon::Error::GetErrorCodeString( this->code );
		if( this->message != "" )
			stream << "\n       Message: " << this->message;
		if( this->ptr )
			stream << "\n       By pointer: " << ((unsigned long long)(this->ptr));
	}
	
	Error & Error::operator = ( const Error & other )
	{
		this->code = other.code;
		this->message = other.message;
		this->ptr = other.ptr;
		return *this;
	}
	
	Error::Error( unsigned code_, const std::string & message_, void * ptr_ ) :
		message(message_), code(code_), ptr(ptr_)
	{
	}
	
	Error::Error() :
		message(), code(ICon::Error::none), ptr(nullptr)
	{
	}
	
	Error::~Error()
	{
		this->code = ICon::Error::none;
		this->ptr = nullptr;
	}
	
	std::string Error::GetErrorCodeString( unsigned code )
	{
		return "Not done yet";
	}
};

#endif

