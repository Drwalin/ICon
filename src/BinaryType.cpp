
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef BINARY_TYPE_CPP
#define BINARY_TYPE_CPP

#include "BinaryType.h"

namespace Binary
{
	namespace Type
	{
		unsigned long long GetOffset( const std::vector < unsigned char > & type, unsigned long long offset )
		{
			switch( type[offset] )
			{
			case RAW:
			case BOOl:
			case INT_8:
			case INT_16:
			case INT_32:
			case INT_64:
			case UINT_8:
			case UINT_16:
			case UINT_32:
			case UINT_64:
			case FLOAT_32:
			case FLOAT_64:
			case FLOAT_80:
			case STRING:
				return offset + 1;
			
			VECTOR:
			SET:
				return GetOffset( type, offset + 1 );
			
			MAP:
				return GetOffset( type, GetOffset( type, offset + 1 ) );
			}
			return 0;
		}
		
		
		// internal types
		
		unsigned long long Get( std::vector < unsigned char > & type, const void * src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::RAW;
			return offset+1;
		}
		
		unsigned long long Get( std::vector < unsigned char > & type, const float src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::FLOAT_32;
			return offset+1;
		}
		
		unsigned long long Get( std::vector < unsigned char > & type, const double src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::FLOAT_64;
			return offset+1;
		}
		
		unsigned long long Get( std::vector < unsigned char > & type, const long double src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::FLOAT_80;
			return offset+1;
		}
		
		unsigned long long Get( std::vector < unsigned char > & type, const bool src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::BOOl;
			return offset+1;
		}
		
		unsigned long long Get( std::vector < unsigned char > & type, const char src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::INT_8;
			return offset+1;
		}
		
		unsigned long long Get( std::vector < unsigned char > & type, const short src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::INT_16;
			return offset+1;
		}
		
		unsigned long long Get( std::vector < unsigned char > & type, const int src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::INT_32;
			return offset+1;
		}
		
		unsigned long long Get( std::vector < unsigned char > & type, const long long src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::INT_64;
			return offset+1;
		}
		
		unsigned long long Get( std::vector < unsigned char > & type, const unsigned char src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::UINT_8;
			return offset+1;
		}
		
		unsigned long long Get( std::vector < unsigned char > & type, const unsigned short src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::UINT_16;
			return offset+1;
		}
		
		unsigned long long Get( std::vector < unsigned char > & type, const unsigned int src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::UINT_32;
			return offset+1;
		}
		
		unsigned long long Get( std::vector < unsigned char > & type, const unsigned long long src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::UINT_64;
			return offset+1;
		}
		
		
		
		// std::string
		unsigned long long Get( std::vector < unsigned char > & type, const std::string & src, unsigned long long offset )
		{
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::STRING;
			return offset+1;
		}
	};
	
	
	
	// internal type
	
	unsigned long long Store( std::vector < unsigned char > & dst, const bool src, unsigned long long offset )
	{
		if( offset + 1 > dst.size() )
			dst.resize( offset + 1 );
		dst[offset] = src ? 255 : 0;
		return offset + 1;
	}
	
	
	
	// std::string
	unsigned long long Store( std::vector < unsigned char > & dst, const std::string & src, unsigned long long offset )
	{
		if( offset + sizeof(Binary::Type::NumberOfElements) + src.size() > dst.size() )
			dst.resize( offset + sizeof(Binary::Type::NumberOfElements) + src.size() );
		Binary::Type::NumberOfElements elements = src.size();
		memmove( &(dst.front()) + offset, (&elements), sizeof(Binary::Type::NumberOfElements) );
		memmove( &(dst.front()) + offset + sizeof(Binary::Type::NumberOfElements), src.c_str(), elements );
		return offset + sizeof(Binary::Type::NumberOfElements) + src.size();
	}
	
	
	
	
	
	// internal types
	
	unsigned long long Restore( const std::vector < unsigned char > & src, bool & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			if( src[offset] )
				dst = true;
			else
				dst = false;
			return offset + 1;
		}
		return 0;
	}
	
	unsigned long long Restore( const std::vector < unsigned char > & src, std::vector<bool>::reference dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			if( src[offset] )
				dst = true;
			else
				dst = false;
			return offset + 1;
		}
		return 0;
	}
	
	
	
	// std::string
	unsigned long long Restore( const std::vector < unsigned char > & src, std::string & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(Binary::Type::NumberOfElements) )
		{
			Binary::Type::NumberOfElements elements = 0;
			memmove( &elements, &(src.front()) + offset, sizeof(Binary::Type::NumberOfElements) );
			if( src.size() >= offset + elements )
			{
				dst.resize( elements+1 );
				dst[elements] = 0;
				memmove( &(dst.front()), &(src.front()) + offset + sizeof(Binary::Type::NumberOfElements), elements );
				return offset + sizeof(Binary::Type::NumberOfElements) + elements;
			}
		}
		return 0;
	}
};

#endif

