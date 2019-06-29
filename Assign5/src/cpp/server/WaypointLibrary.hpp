#include <map>
#include <string>
#include <cmath>
#include <math.h>
#include "Waypoint.hpp"
#include <json/json.h>

class WaypointLibrary {
    public:
	    std::string name;
	    map<std::string, Waypoint> waypoints; //collection of waypoint objects
	    WaypointLibrary();
	    WaypointLibrary(string);
	    ~WaypointLibrary();
	    bool removeWpt(string wpt);
	    bool addWpt(double lat, double lon, double ele, string name, string address);
	    Json::Value modWpt(double lat, double lon, double ele,string nuName, string name, string address);
	    bool expjson_file();
	    Json::Value impjson_file();
	    int get_size();
	    static double bearing(Waypoint from, Waypoint to);
	    static double distance(Waypoint from, Waypoint to);
    private:
	    static double toDegrees(double input);
	    static double toRadians(double input);
};
