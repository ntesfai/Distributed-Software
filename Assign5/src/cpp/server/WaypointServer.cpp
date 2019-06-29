#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <csignal>
#include <json/json.h>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include "waypointserverstub.h"
#include "WaypointLibrary.hpp"

using namespace std;
using namespace jsonrpc;

class WaypointServer : public waypointserverstub {
    public:
        WaypointServer(AbstractServerConnector &connector, int port);
	virtual bool removeWpt(const std::string& param);
	virtual bool addWpt(double param1, double param2, double param3, const std::string& param4, const std::string& param5);
	virtual Json::Value impjson_file();
	virtual Json::Value modWpt(double param1, double param2, double param3, const std::string& param4, const std::string& param5, const std::string& param6);
	virtual bool expjson_file();
	virtual int get_size();
    private:
        WaypointLibrary * library;
	int port_num;
};

int WaypointServer::get_size(){
    cout << "Getting the number of waypoints" << endl;
    return library -> get_size();
}

WaypointServer::WaypointServer(AbstractServerConnector &connector, int port) : waypointserverstub(connector){
    library = new WaypointLibrary("waypoints.json");
    port_num = port;
}

bool WaypointServer::expjson_file(){
    cout << "Exporting json objects to waypoint.json" << endl;
    library -> expjson_file();
}

Json::Value WaypointServer::impjson_file(){
    cout << "Restoring collection from waypoints.json" << endl;
    return library -> impjson_file();
}

bool WaypointServer::removeWpt(const std::string& param) {
    if(library -> removeWpt(param)){
	cout << "Removing: " << param << endl;
        return true;
    }
    else
	return false;
    cout << "Removing:" << param << endl;
    return library -> removeWpt(param);
}

bool WaypointServer::addWpt(double param1, double param2, double param3, const std::string& param4, const std::string& param5) {
    cout << "Adding new Waypoint" << endl;
    return library -> addWpt(param1, param2, param3, param4, param5);
}


Json::Value WaypointServer::modWpt(double param1, double param2, double param3, const std::string& param4, const std::string& param5, const std::string& param6){
    cout << "Adding new Waypoint" << endl;
    return library -> modWpt(param1, param2, param3, param4, param5, param6);
}

void exiting(){
    cout << "Server terminated, exiting..." << endl;
}

int main(int argc, char * argv[]){
    int port = 8080;
    if(argc > 1)
	    port = atoi(argv[1]);
    HttpServer httpserver(port);
    WaypointServer ws(httpserver, port);
    auto ex = [](int i) {cout << "server terminating with signal" << endl;
    exit(0);
    };

    std::signal(SIGINT, ex);
    std::signal(SIGABRT, ex);
    std::signal(SIGTERM, ex);
    std::signal(SIGTSTP, ex);

    cout << "Waypoint browser server listening on port: " << port << endl;
    ws.StartListening();
    while(1);
    ws.StopListening();
    return 0;
}
