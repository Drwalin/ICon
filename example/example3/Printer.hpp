
#include <type_traits>

#include <cstdio>

#include <iostream>
#include <sstream>

#include <vector>
#include <map>
#include <set>

void PrintLN( FILE * f, int offset )
{
	fprintf( f, "\n " );
	for( int i = 0; i < offset; ++i )
		fprintf( f, " " );
}
template < typename T >
void Print( FILE * f, const T val, int offset = 0 );

template < typename T >
void Print( FILE * f, const T val, int offset )
{
	if( std::is_fundamental<T>::value )
	{
		std::stringstream stream;
		stream << "(" << val << ")";
		fprintf( f, stream.str().c_str() );
	}
}

void Print( FILE * f, const char val, int offset = 0 )
{
	fprintf( f, "(%i)", int(val) );
}

void Print( FILE * f, const unsigned char val, int offset = 0 )
{
	fprintf( f, "(%i)", int(val) );
}

void Print( FILE * f, const bool val, int offset = 0 )
{
	fprintf( f, "(%i)", val ? "true" : "false" );
}

void Print( FILE * f, const int val, int offset = 0 )
{
	fprintf( f, "(%i)", val );
}

void Print( FILE * f, const long long val, int offset = 0 )
{
	fprintf( f, "(%lld)", val );
}

void Print( FILE * f, const float val, int offset = 0 )
{
	fprintf( f, "(%f)", val );
}

void Print( FILE * f, const double val, int offset = 0 )
{
	fprintf( f, "(%f)", (float)val );
}

void Print( FILE * f, const std::string & val, int offset = 0 )
{
	fprintf( f, "\"%s\"", val.c_str() );
}

template < typename T1, typename T2 >
void Print( FILE * f, const std::map<T1,T2> & val, int offset = 0 );

template < typename T >
void Print( FILE * f, const std::set<T> & val, int offset = 0 );

template < typename T >
void Print( FILE * f, const std::vector<T> & val, int offset = 0 )
{
	fprintf( f, "( " );
	for( auto it = val.begin(); it != val.end(); ++it )
	{
		Print( f, *it, offset+4 );
		fprintf( f, " ; " );
	}
	fprintf( f, " )" );
}

template < typename T >
void Print( FILE * f, const std::set<T> & val, int offset )
{
	fprintf( f, "( " );
	for( auto it = val.begin(); it != val.end(); ++it )
	{
		Print( f, *it, offset+4 );
		fprintf( f, " ; " );
	}
	fprintf( f, " )" );
}

template < typename T1, typename T2 >
void Print( FILE * f, const std::map<T1,T2> & val, int offset )
{
	fprintf( f, "\n [" );
	for( auto it = val.begin(); it != val.end(); ++it )
	{
		PrintLN( f, offset );
		fprintf( f, "( " );
		Print( f, it->first, offset+4 );
		fprintf( f, " ; " );
		Print( f, it->second, offset+4 );
		fprintf( f, " )" );
	}
	fprintf( f, "\n ]" );
}

void Print( FILE * f, std::vector<unsigned char> val, int offset = 0 )
{
	fprintf( f, "( " );
	for( auto it = val.begin(); it != val.end(); ++it )
	{
		fprintf( f, "(%i)", int(*it) );
		fprintf( f, " ; " );
	}
	fprintf( f, " )" );
}

