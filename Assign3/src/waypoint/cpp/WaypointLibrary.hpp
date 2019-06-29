#include <map>
#include <string>
#include <cmath>
#include <math.h>
#include "Waypoint.hpp"

class WaypointLibrary {
    public:
	    std::string name;
	    map<std::string, Waypoint> waypoints; //collection of waypoint objects
	    WaypointLibrary();
	    WaypointLibrary(string);
	    ~WaypointLibrary();
	    void removeWpt(string wpt);
	    void addWpt(double lat, double lon, double ele, string name, string address);
	    void modWpt(double lat, double lon, double ele,string nuName, string name, string address);
	    void expjson_file();
	    void impjson_file();
	    int getSize();
	    static double bearing(Waypoint from, Waypoint to);
	    static double distance(Waypoint from, Waypoint to);
    private:
	    static double toRadians(double input);
	    static double toDegrees(double input);
};
