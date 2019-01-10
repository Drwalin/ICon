
#include "../../src/Connection.h"
#include "../../src/HighLayerSocket.h"
#include "../../src/DeepServer.h"

#include <cstring>

int main()
{
	ICon::Init();
	ICon::Error::PrintWhenPushed();
	
	{
		std::shared_ptr<ICon::DeepServer> server = ICon::DeepServer::Make();
		std::shared_ptr<ICon::HighLayerSocket> fcon( new ICon::HighLayerSocket() );
		
		std::vector <int> tab;
		server->StartListening( 27000 );
		if( server->Accept( fcon ) == ICon::Error::Code::none )
		{
			printf( "\n Connected correctly! " );
			
			ICon::Connection con( fcon );
			
			con << 311.0f;
			
			std::string str;
			
			con >> str;
			
			printf( "\n Received string: %s ", str.c_str() );
			
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



