
/*
	Copyright (C) 2019 Marek Zalewski aka Drwalin - All Rights Reserved
	
	Any one is authorized to copy, use or modify this file,
	but only when this file is marked as written by Marek Zalewski.
	
	No one can claim this file is originally written by them.
	
	No one can modify or remove this Copyright notice from this file.
*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include "FixedConnection.h"

#include <map>
#include <vector>
#include <set>

#include <memory>

class Connection
{
private:
	
	std::shared_ptr<FixedConnection> con;
	std::vector < unsigned char > buffer;
	
	template < typename T >
	void GetTypeBinary( std::vector < unsigned char > & type );
	template < typename T >
	bool IsTypeValid( const std::vector < unsigned char > & type, const unsigned offset );
	
public:
	
	bool IsValid();
	
	void UpdateReceive();
	bool IsAnythingToRecevie( bool updateReceive = true );
	
	
	template < typename T >
	Connection & operator << ( const T & obj );
	template < typename T >
	Connection & operator >> ( const T & obj );
	
	unsigned write( const void * src, const unsigned size );
	unsigned read( void * dst, const unsigned size );
	
	void SetFixedConnection( std::shared_ptr<FixedConnection> con );
	std::shared_ptr<FixedConnection> GetFixedConnection();
	
	Connection( std::shared_ptr<FixedConnection> con );
	Connection();
	~Connection();
};

#include "Connection.inl"

#endif

