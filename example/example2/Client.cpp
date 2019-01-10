
#include "../../src/Connection.h"
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
			std::shared_ptr<ICon::HighLayerSocket> fcon( new ICon::HighLayerSocket() );
			
			std::vector <int> tab;
			if( fcon->Connect( "127.0.0.1", 27000 ) == ICon::Error::Code::none )
			{
				printf( "\n Connected correctly! " );
				
				ICon::Connection con( fcon );
				
				float f;
				
				con >> f;
				
				printf( "\n received float = %f ", f );
				
				con << std::string( "Zielone mleko xDD" );
				
				
				
				con.Close();
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



