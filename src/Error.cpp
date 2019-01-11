
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
	
	void Error::Push( unsigned code, unsigned lineCode, std::string fileName, const std::string & message, void * ptr )
	{
		ICon::globalErrorQueueModificationMutex.lock();
		ICon::globalErrorQueue.emplace_back( code, lineCode, fileName, message, ptr );
		if( ICon::printErrorWhenPushed )
			ICon::globalErrorQueue.back().Print();
		ICon::globalErrorQueueModificationMutex.unlock();
	}
	
	void Error::Push( unsigned code, unsigned line, std::string fileName )
	{
		ICon::globalErrorQueueModificationMutex.lock();
		ICon::globalErrorQueue.emplace_back( code, line, fileName , "", nullptr );
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
		return Error( ICon::Error::Code::tryingToAccessEmptyErrorQueue, 0, "#invalid#", "", nullptr );
	}
	
	Error::operator unsigned() const
	{
		return this->code;
	}
	
	void Error::Print( std::ostream & stream )
	{
		stream << "\n ICon::Error appeard with code 0x" << std::hex << this->code << ": " << ICon::Error::GetErrorCodeString( this->code );
		stream << "\n       At file: " << this->fileName << ":" << std::dec << this->line;
		if( this->message != "" )
			stream << "\n       Message: " << this->message;
		if( this->ptr )
			stream << "\n       By pointer: 0x" << std::hex << ((unsigned long long)(this->ptr));
		stream << std::dec;
	}
	
	Error & Error::operator = ( const Error & other )
	{
		this->code = other.code;
		this->message = other.message;
		this->ptr = other.ptr;
		this->line = other.line;
		this->fileName = other.fileName;
		return *this;
	}
	
	Error::Error( unsigned code_, unsigned line_, std::string fileName_, const std::string & message_, void * ptr_ ) :
		message(message_), code(code_), ptr(ptr_), line(line_), fileName(fileName_)
	{
	}
	
	Error::Error( const Error & other )
	{
		*this = other;
	}
	
	Error::Error() :
		message(), code(ICon::Error::none), ptr(nullptr), line(0), fileName("none")
	{
	}
	
	Error::~Error()
	{
		this->code = ICon::Error::none;
		this->ptr = nullptr;
	}
	
	std::string Error::GetErrorCodeString( unsigned code )
	{
		switch( code )
		{
		case ICon::Error::Code::none: return "No error code";
		
		case ICon::Error::Code::failedToAccept: return "Failed to accept HighSocketLayer connection";
		case ICon::Error::Code::failedToConnect: return "Failed to connect";
		case ICon::Error::Code::failedToInitIoService: return "Faild to inint boost::asio::io_context ICon::ioService";
		
		case ICon::Error::Code::tryingToSendThorughInvalidConnection: return "Trying to send Connection::operator<< through invalid Connection";
		case ICon::Error::Code::tryingToReceiveFromInvalidConnection: return "Trying to receive Connection::operator>> from invalid Connection";
		
		case ICon::Error::Code::tryingToRunAcceptWhileAcceptNoLockIsRunning: return "Trying to call DeepServer::Accept while DeepServer::AcceptNoLock is running";
		case ICon::Error::Code::tryingToRunSecondInstancOfAcceptNoLock: return "Trying to run second instance of DeepServer::AcceptNoLock in one DeepServer";
		
		case ICon::Error::Code::tryingToDeInintUnexistingContext: return "Trying to DeInit unexisting ICon::ioService";
		case ICon::Error::Code::tryingToAcceptUsingClosedServer: return "Trying to accept by closed DeepServer";
		case ICon::Error::Code::tryingToAccessEmptyErrorQueue: return "Trying to access empty queue (ICon::globalErrorQueue)";
		case ICon::Error::Code::tryingToSendDataByInvalidHighSocketLayer: return "Trying to HighSocketLayer::Send through invalid HighSocketLayer";
		case ICon::Error::Code::tryingToSendInvalidDataSize: return "Trying to HighSocketLayer::Send invalid data size";
		case ICon::Error::Code::tryingToSendInvalidDataPointer: return "Trying to HighSocketLayer::Send invalid pointer";
		case ICon::Error::Code::connectionClosedByErrorWhileSendingData: return "Connection has been closed due to boost::asio::tcp::ip::socket error in HighSocketLayer::Send";
		case ICon::Error::Code::tryingToAcceptToUnallocatedHighSocketLayer: return "Trying to accept connection to invalid std::shared_ptr<HighSocketLayer>";
		case ICon::Error::Code::tryingToReceiveFromInvalidHighLayerSocket: return "Trying to receive message from invalid HighLayerSocket";
		case ICon::Error::Code::connectionBrokenWhileReceiving: return "Connection has been broken while receiving";
		case ICon::Error::Code::tryingToReceiveToInvalidType: return "Trying to receive Connection::operator>> to invalid type";
		case ICon::Error::Code::tryingToPopEmptyMessageStack: return "Trying to pop empty message buffer";
		
		case ICon::Error::Code::failedToReceiveLock: return "Failed to HighSocketLayer::ReceiveLock";
		case ICon::Error::Code::failedToRestoreMessage: return "Failed to Binary::Restore message";
		case ICon::Error::Code::tryingToSendInvalidBuffersArray: return "Trying to HighLevelSocket::Send invalid buffers array";
		case ICon::Error::Code::tryingToSendThroughInvalidConnection: return "Trying to send through invalid Connection";
		case ICon::Error::Code::tryingToReadFromInvalidConnection: return "Trying to read from invalid Connection";
		case ICon::Error::Code::failedToReceiveBuffer: return "Failed to receive message";
		case ICon::Error::Code::receivedInvalidBuffer: return "Invalid messagehas been received";
		case ICon::Error::Code::tryingToReadFromBufferNotOfTypeRaw: return "Trying to Connection::read not from buffer of Binary::Type::RAW type";
		case ICon::Error::Code::failedToReceiveToValidType: return "Failed to receive Connection::operator>> to a valid type";
		
		case ICon::Error::Code::failedToGetMessageLockWhichReturnedConstReference: return "Failed to HighLayerSocket::GetMessageLock which returned const reference";
		case ICon::Error::Code::failedToGetPopMessageLockDueToBoostAsioError: return "Failed to HighLayerSocket::GetPopMessageLock receive due to boost::asio error";
		case ICon::Error::Code::failedToAllocateBoostAsioEndpointOrAcceptor: return "Failed to allocate boost::asio::ip::tcp::endpoint or boost::asio::ip::tcp::acceptor";
		
		case ICon::Error::Code::receivedZeroBytesWhileReceivingNoLock: return "Failed to read_some data while receiving more than 0 bytes";
		
		case ICon::Error::Code::unknown: return "Unknown error";
		};
		
		return "Undefined error code";
	}
};

#endif

