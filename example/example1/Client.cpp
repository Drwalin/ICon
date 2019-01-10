
#include "../../src/HighLayerSocket.h"

#include <boost/exception/all.hpp>

#include <cstring>

int main()
{
	try
	{
		ICon::Init();
		ICon::Error::PrintWhenPushed();
		
		{
			std::shared_ptr<ICon::HighLayerSocket> con( new ICon::HighLayerSocket() );
			
			std::vector <int> tab;
			if( con->Connect( "127.0.0.1", 27000 ) == ICon::Error::Code::none )
			{
				printf( "\n Connected correctly! " );
				
				auto v = con->GetMessageLock();
				tab.resize( v.size() / sizeof(int) );
				memcpy( &tab.front(), &v.front(), tab.size() * sizeof(int) );
				for( int i = 0; i < tab.size(); ++i )
					printf( "\n tab[%i] = %i ", i, tab[i] );
				
				
				tab.resize( 117 );
				for( int i = 0; i < tab.size(); ++i )
					tab[i] = i + 100;
				con->Send( &(tab.front()), tab.size() * sizeof(int) );
				
				con->Close();
			}
			
			while( true )
			{
				ICon::Error err = ICon::Error::Pop();
				if( err == ICon::Error::Code::tryingToAccessEmptyErrorQueue )
					break;
				err.Print();
			}
		}
		
		ICon::DeInit();
	}
	catch( std::exception & e )
	{
		std::cout << "\n Exception: " << e.what() << " \n";
	}
	
	return 0;
}



