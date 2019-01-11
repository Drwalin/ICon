
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef LOOB_BUFFER_H
#define LOOB_BUFFER_H

namespace ICon
{
	class LoopBuffer
	{
	private:
		
		unsigned char ** buffer;
		unsigned long long bufferNumber;
		unsigned long long bufferSize;
		unsigned long long capacity;
		
		unsigned long long offsetBeg;
		unsigned long long offsetEnd;
		
		void CorrectBufferOffset();
		
	public:
		
		bool IsValid();
		
		void Init( unsigned long long bufferNumber = 1024, unsigned long long bufferSize = 1024 );
		void Destroy();
		
		// all these functions assume that this->IsValid() == true
		
		unsigned long long GetBytesStored() const;
		unsigned long long GetCapacity() const;
		unsigned long long GetAvailableFreeBytes() const;
		
		unsigned long long GetEndOffset() const;
		
		unsigned long long GetAvailableLengthAtOnce( const unsigned long long offset ) const;
		unsigned long long GetAvailableLengthAtOnceOnEnd() const;
		unsigned char * GetBegPtr();
		unsigned char * GetEndPtr();
		unsigned char * GetPtr( const unsigned long long offset = 0 );
		void PushData( const unsigned long long length );
		void PushData( void * ptr, unsigned long long length );
		
		void GetData( void * dst_, const unsigned long long length, const unsigned long long offset = 0 ) const;
		void MoveOrigin( const unsigned long long length );
		
		void Clear();
		
		LoopBuffer();
		~LoopBuffer();
	};
};

#endif

