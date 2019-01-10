
#include "Printer.hpp"

#include "../../src/Connection.h"
#include "../../src/HighLayerSocket.h"
#include "../../src/DeepServer.h"

int main()
{
	ICon::Init();
	ICon::Error::PrintWhenPushed();
	
	{
		std::shared_ptr<ICon::DeepServer> server = ICon::DeepServer::Make();
		std::shared_ptr<ICon::HighLayerSocket> fcon( new ICon::HighLayerSocket() );
		
		server->StartListening( 27000 );
		if( server->Accept( fcon ) == ICon::Error::Code::none )
		{
			printf( "\n Connected correctly! " );
			
			ICon::Connection con( fcon );
			
			con << 311.0f;
			
			std::string str;
			
			con >> str;
			
			printf( "\n Received string: %s ", str.c_str() );
			
			
			
			std::map < std::vector<int>, std::vector<std::string> > testVal =
			{
				{
					{
						1, 2, 3, 4, 5, 6, 7, 8
					},
					{
						"a_1", "b_1", "c_1"
					}
				},
				{
					{
						11, 12, 13, 14, 15, 16, 17, 18
					},
					{
						"a_2", "b_2", "c_2"
					}
				},
				{
					{
						21, 22, 23, 24, 25, 26, 27, 28
					},
					{
						"a_3", "b_3", "c_3"
					}
				},
				{
					{
						31, 32, 33, 34, 35, 36, 37, 38
					},
					{
						"a_4", "b_4", "c_4"
					}
				},
			};
			
			con << testVal;
			
			Print( stderr, testVal );
			
			con.Close();
		}
		
		while( true )
		{
			ICon::Error err = ICon::Error::Pop();
			if( err == ICon::Error::Code::tryingToAccessEmptyErrorQueue )
				break;
			err.Print();
		}
		
		server->Close();
	}
	
	ICon::DeInit();
	
	return 0;
}



