
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef LOOB_BUFFER_CPP
#define LOOB_BUFFER_CPP

#include "LoopBuffer.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

inline unsigned long long __fastcall MINULL( unsigned long long a, unsigned long long b )
{
	if( a < b )
		return a;
	return b;
}

namespace ICon
{
	
	void LoopBuffer::CorrectBufferOffset()
	{
		while( this->offsetEnd >= this->capacity && this->offsetBeg >= this->capacity )
		{
			this->offsetBeg -= this->capacity;
			this->offsetEnd -= this->capacity;
		}
		if( this->offsetBeg + this->capacity < this->offsetEnd )
			fprintf( stderr, "\n LoopBuffer::PushData buffer overflow by amount of: %llu bytes", this->offsetEnd - this->offsetBeg - this->capacity );
	}
	
	bool LoopBuffer::IsValid()
	{
		return ( this->buffer != nullptr && bufferNumber != 0 && bufferSize != 0 );
	}
	
	void LoopBuffer::Init( unsigned long long bufferNumber, unsigned long long bufferSize )
	{
		this->Destroy();
		this->bufferNumber = bufferNumber;
		this->bufferSize = bufferSize;
		this->buffer = (unsigned char**)malloc( sizeof(unsigned char*) * this->bufferNumber );
		unsigned char ** ptr = this->buffer;
		unsigned char ** end = this->buffer + ( this->bufferNumber );
		for( ; ptr < end; ++ptr )
			*ptr = (unsigned char*)malloc( this->bufferSize );
		this->capacity = this->bufferSize * this->bufferNumber;
	}
	
	void LoopBuffer::Destroy()
	{
		if( this->buffer )
		{
			unsigned char ** ptr = this->buffer;
			unsigned char ** end = this->buffer + ( this->bufferNumber );
			for( ; ptr < end; ++ptr )
			{
				if( *ptr != nullptr )
					free( *ptr );
			}
			free( this->buffer );
			this->buffer = nullptr;
		}
		this->bufferNumber = 0;
		this->bufferSize = 0;
		this->offsetBeg = 0;
		this->offsetEnd = 0;
	}
	
	
	unsigned long long LoopBuffer::GetBytesStored() const
	{
		return this->offsetEnd - this->offsetBeg;
	}
	
	unsigned long long LoopBuffer::GetCapacity() const
	{
		return this->capacity;
	}
	
	unsigned long long LoopBuffer::GetAvailableFreeBytes() const
	{
		return this->capacity - this->GetBytesStored();
	}
	
	unsigned long long LoopBuffer::GetEndOffset() const
	{
		return this->offsetEnd - this->offsetBeg;
	}
	
	unsigned long long LoopBuffer::GetAvailableLengthAtOnce( const unsigned long long offset ) const
	{
		return MINULL( this->bufferSize - ( ( this->offsetBeg + offset ) % this->bufferSize ), this->GetAvailableFreeBytes() );
	}
	
	unsigned long long LoopBuffer::GetAvailableLengthAtOnceOnEnd() const
	{
		return MINULL( this->bufferSize - ( this->offsetEnd % this->bufferSize ), this->GetAvailableFreeBytes() );
	}
	
	unsigned char * LoopBuffer::GetBegPtr()
	{
		return this->buffer[ ( this->offsetBeg / this->bufferSize ) % this->bufferNumber ] + ( this->offsetBeg % this->bufferSize );
	}
	
	unsigned char * LoopBuffer::GetEndPtr()
	{
		return this->buffer[ ( this->offsetEnd / this->bufferSize ) % this->bufferNumber ] + ( this->offsetEnd % this->bufferSize );
	}
	
	unsigned char * LoopBuffer::GetPtr( const unsigned long long offset )
	{
		unsigned long long globalOffset = this->offsetBeg + offset;
		return this->buffer[ ( globalOffset / this->bufferSize ) % this->bufferNumber ] + ( globalOffset % this->bufferSize );
	}
	
	void LoopBuffer::PushData( const unsigned long long length )
	{
		this->offsetEnd += length;
		this->CorrectBufferOffset();
	}
	
	void LoopBuffer::PushData( void * ptr, unsigned long long length )
	{
		if( this->GetAvailableFreeBytes() >= length )
		{
			while( length != 0 )
			{
				unsigned long long current = MINULL( length, this->GetAvailableLengthAtOnceOnEnd() );
				memmove( this->GetEndPtr(), ptr, current );
				length -= current;
				this->PushData( current );
			}
		}
		else
		{
		}
	}
	
	void LoopBuffer::GetData( void * dst_, const unsigned long long length, const unsigned long long offset ) const
	{
		unsigned char * dst = (unsigned char*)dst_;
		unsigned long long bytesToCopy = length;
		unsigned long long i = this->offsetBeg + offset;
		unsigned long long buf = i / this->bufferSize;
		
		unsigned long long copy;
		
		copy = MINULL( bytesToCopy, this->bufferSize - ( i % this->bufferSize ) );
		memmove( dst, this->buffer[ ( i / this->bufferSize ) % this->bufferNumber ] + ( i % this->bufferSize ), copy );
		bytesToCopy -= copy;
		dst += copy;
		
		while( bytesToCopy != 0 )
		{
			while( buf > this->bufferNumber )
				buf -= this->bufferNumber;
			copy = MINULL( bytesToCopy, this->bufferSize );
			memmove( dst, this->buffer[buf], copy );
			bytesToCopy -= copy;
			dst += copy;
			++buf;
		}
	}
	
	void LoopBuffer::MoveOrigin( const unsigned long long length )
	{
		this->offsetBeg += length;
		if( this->offsetBeg > this->offsetEnd )
			fprintf( stderr, "\n LoopBuffer::MoveOrigin more bytes than was stored has been released by amount of: %llu bytes", this->offsetBeg - this->offsetEnd );
		this->CorrectBufferOffset();
	}
	
	void LoopBuffer::Clear()
	{
		this->offsetBeg = 0;
		this->offsetEnd = 0;
	}
	
	LoopBuffer::LoopBuffer()
	{
		buffer = nullptr;
		bufferNumber = 0;
		bufferSize = 0;
		offsetBeg = 0;
		offsetEnd = 0;
	}
	
	LoopBuffer::~LoopBuffer()
	{
		Destroy();
	}
};

#undef MIN

#endif

