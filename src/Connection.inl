
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef CONNECTION_INL
#define CONNECTION_INL

//template < typename T >
void Connection::GetTypeBinary<>( std::vector < unsigned char > & type )


template < typename T >
void Connection::GetTypeBinary( std::vector < unsigned char > & type )

template < typename T >
void Connection::GetTypeBinary( std::vector < unsigned char > & type )
{
	
	
	
	
}


template < typename T >
bool Connection::IsTypeValid( const std::vector < unsigned char > & type, const unsigned offset )
{
	std::vector < unsigned char > other;
	GetTypeBinary<T>( other );
	return std::equal( other.begin(), other.end(), type.begin() + offset );
}







#endif

