
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
		// internal types
		
		/*
		template < typename T >
		unsigned long long Get( std::vector < unsigned char > & type, const T src, unsigned long long offset = 0 )
		{
			if( std::is_fundamental<T>::value )
			{
				if( offset+1 > type.size() )
					type.resize( offset+1 );
				type[offset] = Binary::Type::FLOAT_32;
				return offset+1;
			}
			return 0;
		}
		*/
		
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
		inline bool IsValid( const std::vector < unsigned char > & type, const T & src, unsigned long long offset )
		{
			std::vector < unsigned char > temp;
			temp.reserve( 32 );
			Get( temp, src, 0 );
			return std::equal( temp.begin(), temp.end(), type.begin() + offset );
		}
	};
	
	
	
	// internal types
	
	/*
	template < typename T >
	unsigned long long Store( std::vector < unsigned char > & dst, const T src, unsigned long long offset = 0 )
	{
		if( std::is_fundamental<T>::value )
		{
			if( offset + sizeof(src) > dst.size() )
				dst.resize( offset + sizeof(src) );
			memmove( &(dst.front()) + offset, &src, sizeof(src) );
			return offset + sizeof(src);
		}
		return 0;
	}
	*/
	
	unsigned long long Store( std::vector < unsigned char > & dst, const float src, unsigned long long offset )
	{
		if( offset + sizeof(src) > dst.size() )
			dst.resize( offset + sizeof(src) );
		memmove( &(dst.front()) + offset, &src, sizeof(src) );
		return offset + sizeof(src);
	}
	
	unsigned long long Store( std::vector < unsigned char > & dst, const double src, unsigned long long offset )
	{
		if( offset + sizeof(src) > dst.size() )
			dst.resize( offset + sizeof(src) );
		memmove( &(dst.front()) + offset, &src, sizeof(src) );
		return offset + sizeof(src);
	}
	
	unsigned long long Store( std::vector < unsigned char > & dst, const long double src, unsigned long long offset )
	{
		if( offset + sizeof(src) > dst.size() )
			dst.resize( offset + sizeof(src) );
		memmove( &(dst.front()) + offset, &src, sizeof(src) );
		return offset + sizeof(src);
	}
	
	unsigned long long Store( std::vector < unsigned char > & dst, const bool src, unsigned long long offset )
	{
		if( offset + 1 > dst.size() )
			dst.resize( offset + 1 );
		dst[offset] = src ? 255 : 0;
		return offset + 1;
	}
	
	unsigned long long Store( std::vector < unsigned char > & dst, const char src, unsigned long long offset )
	{
		if( offset + sizeof(src) > dst.size() )
			dst.resize( offset + sizeof(src) );
		memmove( &(dst.front()) + offset, &src, sizeof(src) );
		return offset + sizeof(src);
	}
	
	unsigned long long Store( std::vector < unsigned char > & dst, const short src, unsigned long long offset )
	{
		if( offset + sizeof(src) > dst.size() )
			dst.resize( offset + sizeof(src) );
		memmove( &(dst.front()) + offset, &src, sizeof(src) );
		return offset + sizeof(src);
	}
	
	unsigned long long Store( std::vector < unsigned char > & dst, const int src, unsigned long long offset )
	{
		if( offset + sizeof(src) > dst.size() )
			dst.resize( offset + sizeof(src) );
		memmove( &(dst.front()) + offset, &src, sizeof(src) );
		return offset + sizeof(src);
	}
	
	unsigned long long Store( std::vector < unsigned char > & dst, const long long src, unsigned long long offset )
	{
		if( offset + sizeof(src) > dst.size() )
			dst.resize( offset + sizeof(src) );
		memmove( &(dst.front()) + offset, &src, sizeof(src) );
		return offset + sizeof(src);
	}
	
	unsigned long long Store( std::vector < unsigned char > & dst, const unsigned char src, unsigned long long offset )
	{
		if( offset + sizeof(src) > dst.size() )
			dst.resize( offset + sizeof(src) );
		memmove( &(dst.front()) + offset, &src, sizeof(src) );
		return offset + sizeof(src);
	}
	
	unsigned long long Store( std::vector < unsigned char > & dst, const unsigned short src, unsigned long long offset )
	{
		if( offset + sizeof(src) > dst.size() )
			dst.resize( offset + sizeof(src) );
		memmove( &(dst.front()) + offset, &src, sizeof(src) );
		return offset + sizeof(src);
	}
	
	unsigned long long Store( std::vector < unsigned char > & dst, const unsigned int src, unsigned long long offset )
	{
		if( offset + sizeof(src) > dst.size() )
			dst.resize( offset + sizeof(src) );
		memmove( &(dst.front()) + offset, &src, sizeof(src) );
		return offset + sizeof(src);
	}
	
	unsigned long long Store( std::vector < unsigned char > & dst, const unsigned long long src, unsigned long long offset )
	{
		if( offset + sizeof(src) > dst.size() )
			dst.resize( offset + sizeof(src) );
		memmove( &(dst.front()) + offset, &src, sizeof(src) );
		return offset + sizeof(src);
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
	
	// std::vector
	template < typename T >
	unsigned long long Store( std::vector < unsigned char > & dst, const std::vector < T > & src, unsigned long long offset )
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
	
	
	
	
	
	// internal types
	
	/*
	template < typename T >
	unsigned long long Restore( const std::vector < unsigned char > & src, T & dst, unsigned long long offset = 0 )
	{
		if( std::is_fundamental<T>::value )
		{
			if( src.size() >= offset + sizeof(dst) )
			{
				memmove( &dst, &(src.front()) + offset, sizeof(dst) );
				return offset + sizeof(dst);
			}
		}
		return 0;
	}
	*/
	
	unsigned long long Restore( const std::vector < unsigned char > & src, float & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			memmove( &dst, &(src.front()) + offset, sizeof(dst) );
			return offset + sizeof(dst);
		}
		return 0;
	}
	
	unsigned long long Restore( const std::vector < unsigned char > & src, double & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			memmove( &dst, &(src.front()) + offset, sizeof(dst) );
			return offset + sizeof(dst);
		}
		return 0;
	}
	
	unsigned long long Restore( const std::vector < unsigned char > & src, long double & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			memmove( &dst, &(src.front()) + offset, sizeof(dst) );
			return offset + sizeof(dst);
		}
		return 0;
	}
	
	unsigned long long Restore( const std::vector < unsigned char > & src, bool & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			memmove( &dst, &(src.front()) + offset, sizeof(dst) );
			return offset + sizeof(dst);
		}
		return 0;
	}
	
	unsigned long long Restore( const std::vector < unsigned char > & src, char & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			memmove( &dst, &(src.front()) + offset, sizeof(dst) );
			return offset + sizeof(dst);
		}
		return 0;
	}
	
	unsigned long long Restore( const std::vector < unsigned char > & src, short & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			memmove( &dst, &(src.front()) + offset, sizeof(dst) );
			return offset + sizeof(dst);
		}
		return 0;
	}
	
	unsigned long long Restore( const std::vector < unsigned char > & src, int & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			memmove( &dst, &(src.front()) + offset, sizeof(dst) );
			return offset + sizeof(dst);
		}
		return 0;
	}
	
	unsigned long long Restore( const std::vector < unsigned char > & src, long long & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			memmove( &dst, &(src.front()) + offset, sizeof(dst) );
			return offset + sizeof(dst);
		}
		return 0;
	}
	
	unsigned long long Restore( const std::vector < unsigned char > & src, unsigned char & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			memmove( &dst, &(src.front()) + offset, sizeof(dst) );
			return offset + sizeof(dst);
		}
		return 0;
	}
	
	unsigned long long Restore( const std::vector < unsigned char > & src, unsigned short & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			memmove( &dst, &(src.front()) + offset, sizeof(dst) );
			return offset + sizeof(dst);
		}
		return 0;
	}
	
	unsigned long long Restore( const std::vector < unsigned char > & src, unsigned int & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			memmove( &dst, &(src.front()) + offset, sizeof(dst) );
			return offset + sizeof(dst);
		}
		return 0;
	}
	
	unsigned long long Restore( const std::vector < unsigned char > & src, unsigned long long & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(dst) )
		{
			memmove( &dst, &(src.front()) + offset, sizeof(dst) );
			return offset + sizeof(dst);
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
				std::copy( src.begin() + offset + sizeof(Binary::Type::NumberOfElements), src.begin() + offset + sizeof(Binary::Type::NumberOfElements) + elements, dst.data() );
				return offset + sizeof(Binary::Type::NumberOfElements) + elements;
			}
		}
		return 0;
	}
	
	// std::vector
	template < typename T >
	unsigned long long Restore( std::vector < unsigned char > & src, std::vector < T > & dst, unsigned long long offset )
	{
		if( src.size() >= offset + sizeof(Binary::Type::NumberOfElements) )
		{
			Binary::Type::NumberOfElements elements = 0;
			memmove( &elements, &(src.front()) + offset, sizeof(Binary::Type::NumberOfElements) );
			dst.resize( elements );
			if( src.size() >= offset + sizeof(Binary::Type::NumberOfElements) + elements )
			{
				unsigned long long tempOffset = offset + sizeof(Binary::Type::NumberOfElements);
				for( auto it = &(dst.front()); it <= &(dst.back()) && tempOffset != 0; ++it )
				{
					tempOffset = Binary::Restore( src, *it, tempOffset );
				}
				return tempOffset;
			}
		}
		return 0;
	}
	
	// std::map
	template < typename T1, typename T2  >
	unsigned long long Restore( std::vector < unsigned char > & src, std::map < T1, T2 > & dst, unsigned long long offset )
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
						break;
					}
					tempOffset = Binary::Restore( src, dst[temp1], tempOffset );
					if( tempOffset == 0 )
					{
						break;
					}
				}
				return tempOffset;
			}
		}
		return 0;
	}
	
	// std::set
	template < typename T >
	unsigned long long Restore( std::vector < unsigned char > & src, std::set < T > & dst, unsigned long long offset )
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

