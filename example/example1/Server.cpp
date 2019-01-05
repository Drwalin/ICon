
//#include "../../src/DeepServer.h"
//#include "../../src/FixedConnection.h"

#include "../Source.cpp"

#include <cstring>

int main()
{
	ICon::Init();
	ICon::Error::PrintWhenPushed();
	
	{
		std::shared_ptr<ICon::DeepServer> server = ICon::DeepServer::Make();
		std::shared_ptr<ICon::FixedConnection> con( new ICon::FixedConnection() );
		
		std::vector <int> tab;
		server->StartListening( 27000 );
		if( server->Accept( con ) == ICon::Error::Code::none )
		{
			printf( "\n Connected correctly! " );
			
			tab.resize( 10 );
			for( int i = 0; i < tab.size(); ++i )
				tab[i] = i + 10;
			con->Send( &(tab.front()), tab.size() * sizeof(int) );
			
			
			auto v = con->GetMessageLock();
			tab.resize( v.size() / sizeof(int) );
			memcpy( &tab.front(), &v.front(), tab.size() * sizeof(int) );
			for( int i = 0; i < tab.size(); ++i )
				printf( "\n tab[%i] = %i ", i, tab[i] );
			
			
			con->Close();
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



