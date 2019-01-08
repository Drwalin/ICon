
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef BINARY_TYPE_INL
#define BINARY_TYPE_INL

namespace Binary
{
	namespace Type
	{
		// std::vector
		template < typename T >
		unsigned long long Get( std::vector < unsigned char > & type, const std::vector < T > & src, unsigned long long offset )
		{
			type.reserve( ((offset>>5)+2)<<5 );
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::VECTOR;
			return Binary::Type::Get( type, T(), offset+1 );
		}
		
		// std::map
		template < typename T1, typename T2  >
		unsigned long long Get( std::vector < unsigned char > & type, const std::map < T1, T2 > & src, unsigned long long offset )
		{
			type.reserve( ((offset>>6)+2)<<6 );
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::MAP;
			unsigned long long tempOffset = Binary::Type::Get( type, T1(), offset+1 );
			return Binary::Type::Get( type, T2(), tempOffset );
		}
		
		// std::set
		template < typename T >
		unsigned long long Get( std::vector < unsigned char > & type, const std::set < T > & src, unsigned long long offset )
		{
			type.reserve( ((offset>>4)+2)<<4 );
			if( offset+1 > type.size() )
				type.resize( offset+1 );
			type[offset] = Binary::Type::SET;
			return Binary::Type::Get( type, T(), offset+1 );
		}
		
		
		
		
		
		template < typename T >
		bool IsValid( const std::vector < unsigned char > & type, const T & src, unsigned long long offset )
		{
			std::vector < unsigned char > temp;
			temp.reserve( 64 );
			unsigned long long size = Get( temp, src, 0 );
			
			if( type.size() < size + offset )
				return false;
			
			unsigned char *b = (unsigned char*)(&(type[offset])), *a = (unsigned char*)(&(temp[0])), *enda = a + size;
			for( ; a < enda && *a == *b; ++a, ++b );
			if( a == enda )
				return true;
			return false;
		}
	};
	
	
	
	
	
	// std::vector
	template < typename T >
	unsigned long long Store( std::vector < unsigned char > & dst, const std::vector < T > & src, unsigned long long offset )
	{
		if( offset + sizeof(Binary::Type::NumberOfElements) + src.size() > dst.size() )
			dst.resize( offset + sizeof(Binary::Type::NumberOfElements) + src.size() );
		Binary::Type::NumberOfElements elements = src.size();
		memmove( &(dst.front()) + offset, (&elements), sizeof(Binary::Type::NumberOfElements) );
		unsigned long long tempOffset = offset + sizeof(Binary::Type::NumberOfElements);
		for( unsigned long long i = 0; i < elements; ++i )
		{
			tempOffset = Binary::Store( dst, src[i], tempOffset );
		}
		return tempOffset;
	}
	
	// std::map
	template < typename T1, typename T2  >
	unsigned long long Store( std::vector < unsigned char > & dst, const std::map < T1, T2 > & src, unsigned long long offset )
	{
		if( offset + sizeof(Binary::Type::NumberOfElements) + src.size() > dst.size() )
			dst.resize( offset + sizeof(Binary::Type::NumberOfElements) + src.size() );
		Binary::Type::NumberOfElements elements = src.size();
		memmove( &(dst.front()) + offset, (&elements), sizeof(Binary::Type::NumberOfElements) );
		unsigned long long tempOffset = offset + sizeof(Binary::Type::NumberOfElements);
		for( auto it = src.begin(); it != src.end(); ++it )
		{
			tempOffset = Binary::Store( dst, it->first, tempOffset );
			tempOffset = Binary::Store( dst, it->second, tempOffset );
		}
		return tempOffset;
	}
	
	// std::set
	template < typename T >
	unsigned long long Store( std::vector < unsigned char > & dst, const std::set < T > & src, unsigned long long offset )
	{
		if( offset + sizeof(Binary::Type::NumberOfElements) + src.size() > dst.size() )
			dst.resize( offset + sizeof(Binary::Type::NumberOfElements) + src.size() );
		Binary::Type::NumberOfElements elements = src.size();
		memmove( &(dst.front()) + offset, (&elements), sizeof(Binary::Type::NumberOfElements) );
		unsigned long long tempOffset = offset + sizeof(Binary::Type::NumberOfElements);
		for( auto it = src.begin(); it != src.end(); ++it )
		{
			tempOffset = Binary::Store( dst, *it, tempOffset );
		}
		return tempOffset;
	}
	
	
	
	
	
	// std::vector
	template < typename T >
	unsigned long long Restore( const std::vector < unsigned char > & src, std::vector < T > & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(Binary::Type::NumberOfElements) )
		{
			Binary::Type::NumberOfElements elements = 0;
			memmove( &elements, &(src.front()) + offset, sizeof(Binary::Type::NumberOfElements) );
			dst.resize( elements );
			if( src.size() >= offset + sizeof(Binary::Type::NumberOfElements) + elements )
			{
				unsigned long long tempOffset = offset + sizeof(Binary::Type::NumberOfElements);
				for( unsigned long long i = 0; i < elements; ++i )
				{
					tempOffset = Binary::Restore( src, dst[i], tempOffset );
				}
				printf( " (std::map->1:%llu)", tempOffset );
				return tempOffset;
			}
			printf( " (std::map->2:0)" );
		}
		printf( " (std::map->3:0)" );
		return 0;
	}
	
	// std::map
	template < typename T1, typename T2  >
	unsigned long long Restore( const std::vector < unsigned char > & src, std::map < T1, T2 > & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(Binary::Type::NumberOfElements) )
		{
			Binary::Type::NumberOfElements elements = 0;
			memmove( &elements, &(src.front()) + offset, sizeof(Binary::Type::NumberOfElements) );
			if( src.size() >= offset + sizeof(Binary::Type::NumberOfElements) + elements )
			{
				unsigned long long tempOffset = offset + sizeof(Binary::Type::NumberOfElements);
				for( Binary::Type::NumberOfElements i = 0; i < elements; ++i )
				{
					T1 temp1;
					tempOffset = Binary::Restore( src, temp1, tempOffset );
					if( tempOffset == 0 )
					{
						printf( " (std::map->1:0)" );
						break;
					}
					tempOffset = Binary::Restore( src, dst[temp1], tempOffset );
					if( tempOffset == 0 )
					{
						printf( " (std::map->2:0)" );
						break;
					}
				}
				printf( " (std::map->3:%llu)", tempOffset );
				return tempOffset;
			}
		}
		printf( " (std::map->4:0)" );
		return 0;
	}
	
	// std::set
	template < typename T >
	unsigned long long Restore( const std::vector < unsigned char > & src, std::set < T > & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(Binary::Type::NumberOfElements) )
		{
			Binary::Type::NumberOfElements elements_ = 0;
			memmove( &elements_, &(src.front()) + offset, sizeof(Binary::Type::NumberOfElements) );
			const Binary::Type::NumberOfElements elements = elements_;
			if( src.size() >= offset + sizeof(Binary::Type::NumberOfElements) + elements )
			{
				unsigned long long tempOffset = offset + sizeof(Binary::Type::NumberOfElements);
				for( Binary::Type::NumberOfElements i = 0; i < elements; ++i )
				{
					T temp;
					tempOffset = Binary::Restore( src, temp, tempOffset );
					if( tempOffset == 0 )
					{
						return 0;
					}
					dst.insert( temp );
				}
				return tempOffset;
			}
		}
		return 0;
	}
};

#endif

