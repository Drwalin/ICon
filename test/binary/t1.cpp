
#include <exception>
#include <iostream>

#include <cstdio>

#include "../../src/BinaryType.cpp"

void PrintLN( FILE * f, int offset )
{
	fprintf( f, "\n " );
	for( int i = 0; i < offset; ++i )
		fprintf( f, " " );
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



int main()
{
	std::map < std::vector < std::set < std::vector<std::string> > >, std::map < std::vector<int>, std::set<double> > > val2;
	std::map < std::vector < std::set < std::vector<std::string> > >, std::map < std::vector<long long>, std::set<float> > > val1 =	{ // map
																																	{ // map::pair
																																		{ // vector
																																			{ // set
																																				{
																																					{"mleko1111"},
																																					{"mielone1111"},
																																					{"zielone1111"}
																																				},
																																				{
																																					{"mleko1112"},
																																					{"mielone1112"},
																																					{"zielone1112"}
																																				}
																																			},
																																			{ // set
																																				{
																																					{"mleko1121"},
																																					{"mielone1121"},
																																					{"zielone1121"}
																																				},
																																				{
																																					{"mleko1122"},
																																					{"mielone1122"},
																																					{"zielone1122"}
																																				}
																																			},
																																			{ // set
																																				{
																																					{"mleko1131"},
																																					{"mielone1131"},
																																					{"zielone1131"}
																																				},
																																				{
																																					{"mleko1132"},
																																					{"mielone1132"},
																																					{"zielone1132"}
																																				}
																																			},
																																		},
																																		{ // map
																																			{ // map::pair
																																				{ // vector
																																					true,
																																					false,
																																					false,
																																					true,
																																					true,
																																					true
																																				},
																																				{ // set
																																					6.0f,
																																					7.0f,
																																					8.0f,
																																					9.0f,
																																				}
																																			}
																																		}
																																	},
																																	{ // map::pair
																																		{ // vector
																																			{ // set
																																				{
																																					{"mleko1211"},
																																					{"mielone1211"},
																																					{"zielone1211"}
																																				},
																																				{
																																					{"mleko1212"},
																																					{"mielone1212"},
																																					{"zielone1212"}
																																				}
																																			},
																																			{ // set
																																				{
																																					{"mleko1221"},
																																					{"mielone1221"},
																																					{"zielone1221"}
																																				},
																																				{
																																					{"mleko1222"},
																																					{"mielone1222"},
																																					{"zielone1222"}
																																				}
																																			},
																																			{ // set
																																				{
																																					{"mleko1231"},
																																					{"mielone1231"},
																																					{"zielone1231"}
																																				},
																																				{
																																					{"mleko1232"},
																																					{"mielone1232"},
																																					{"zielone1232"}
																																				}
																																			},
																																		},
																																		{ // map
																																			{ // map::pair
																																				{ // vector
																																					false,
																																					false,
																																					false,
																																					true,
																																					false,
																																					false,
																																					true,
																																					true,
																																					true
																																				},
																																				{ // set
																																					16.0f,
																																					17.0f,
																																					18.0f,
																																					19.0f,
																																				}
																																			}
																																		}
																																	},
																																	{ // map::pair
																																		{ // vector
																																			{ // set
																																				{
																																					{"mleko1311"},
																																					{"mielone1311"},
																																					{"zielone1311"}
																																				},
																																				{
																																					{"mleko1312"},
																																					{"mielone1312"},
																																					{"zielone1312"}
																																				}
																																			},
																																			{ // set
																																				{
																																					{"mleko1321"},
																																					{"mielone1321"},
																																					{"zielone1321"}
																																				},
																																				{
																																					{"mleko1322"},
																																					{"mielone1322"},
																																					{"zielone1322"}
																																				}
																																			},
																																			{ // set
																																				{
																																					{"mleko1331"},
																																					{"mielone1331"},
																																					{"zielone1331"}
																																				},
																																				{
																																					{"mleko1332"},
																																					{"mielone1332"},
																																					{"zielone1332"}
																																				}
																																			},
																																		},
																																		{ // map
																																			{ // map::pair
																																				{ // vector
																																					true,
																																					true,
																																					true,
																																					true,
																																					true,
																																					false,
																																					false,
																																					true,
																																					false,
																																					false
																																				},
																																				{ // set
																																					26.0f,
																																					27.0f,
																																					28.0f,
																																					29.0f,
																																				}
																																			}
																																		}
																																	},
																																	
																																};
	std::vector < unsigned char > type;
	
	FILE * f1 = fopen( "f1.txt", "w" );
	FILE * f2 = fopen( "f2.txt", "w" );
	
	Binary::Type::Get( type, val1 );
	
	if( Binary::Type::IsValid( type, val1, 0 ) )
		printf( "\n val1 == val1" );
	else
		printf( "\n val1 != val1" );
	
	if( Binary::Type::IsValid( type, val2, 0 ) )
		printf( "\n val1 == val2" );
	else
		printf( "\n val1 != val2" );
	
	
	Print( f1, val1 );
	Print( stderr, val1 );
	
	std::vector < unsigned char > data;
	
	data.reserve( 1024*256 );
	Binary::Store( data, val1 );
	
	val1 = decltype(val1)();
	
	printf( "\n Data size = %llu ", data.size() );
	
	unsigned long long restored = 0;
	
	try
	{
		restored = Binary::Restore( data, val1 );
	}
	catch( const std::exception & e )
	{
		printf( "\n Exception: %s ", e.what() );
	}
	
	printf( "\n Restored offset: %llu", restored );
	
	Print( f2, val1 );
	Print( stderr, val1 );
	
	
	
	
	fclose( f1 );
	fclose( f2 );
	
	
	
	
	f1 = fopen( "f1.txt", "rb" );
	f2 = fopen( "f2.txt", "rb" );
	
	while( !feof(f1) && !feof(f2) )
	{
		char a, b;
		fread( &a, 1, 1, f1 );
		fread( &b, 1, 1, f2 );
		if( a != b )
			printf( "\n   %c != %c ", a, b );
	}
	
	fclose( f1 );
	fclose( f2 );
	
	
	
	
	return 0;
}
