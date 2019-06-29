#include "WaypointLibrary.hpp"
#include <iostream>
#include <json/json.h>
#include <fstream>


WaypointLibrary::WaypointLibrary() {}

/*Create an instance of the WaypointLibrary and initialize it*/
WaypointLibrary::WaypointLibrary(string jsonFile_path) {
   ifstream jsonF(jsonFile_path.c_str(), std::ifstream::binary); 
   Json::Reader reader;
   Json::Value root;
   bool parsed = reader.parse(jsonFile_path, root, false);
   if(parsed) {
       Json::Value::Members mbrs = root.getMemberNames();
       for(vector<string>::const_iterator it = mbrs.begin(); it != mbrs.end(); it++) {
	       Json::Value jsonWpt = root[*it];//insert value from root
               double lat = jsonWpt["lat"].asDouble();
	       double lon = jsonWpt["lon"].asDouble();
	       double ele = jsonWpt["ele"].asDouble();
	       string name = jsonWpt["name"].asString();
	       string address = jsonWpt["address"].asString();
	       Waypoint *temp = new Waypoint(lat, lon, ele, name, address);
	       //waypoints.insert(name, temp);//won't insert duplicates
	       waypoints[*it] = *temp;
       } 
   }
}

//erase a waypoint from the map
void WaypointLibrary::removeWpt(string wpt) {
    waypoints.erase(wpt); 
}

//add a new waypoint to the map
void WaypointLibrary::addWpt(double lat, double lon, double ele, string name, string address) {
    Waypoint *temp = new Waypoint(lat, lon, ele, name, address);
    //waypoints[name] = *temp;
    waypoints.insert(std::pair<string, Waypoint>(name, *temp));
}

//modify a waypoint in the map
void WaypointLibrary::modWpt(double lat, double lon, double ele, string nuName, string name, string address) {
    if(waypoints.count(name) > 0) {
        Waypoint *temp = new Waypoint(lat, lon, ele, nuName, address);
	waypoints.erase(name);
	waypoints.insert(std::pair<string, Waypoint>(nuName, *temp));
    }
    else
	std::cout << "Key: " << name << " not in collection" << endl;
}

//import a set of json objects
void WaypointLibrary::impjson_file() {
   std::string jsonFile_path("waypoints.json");
   std::ifstream jsonF(jsonFile_path.c_str(),std::ifstream::binary);
   if(jsonF.is_open()) {
       cout << "Opened waypoint.json file" << endl;
   }
   else {
       cout << "Failed to open file!" << endl;
       exit(1);
   }
   Json::Reader reader;
   Json::Value root;
   bool parsed = reader.parse(jsonF, root, true);
   if(parsed) {
       Json::Value::Members mbrs = root.getMemberNames();
       for(vector<string>::const_iterator it = mbrs.begin(); it != mbrs.end(); it++) {
	     Json::Value jsonWpt = root[*it];//insert value from root
	     string nameStr = "name";
             double lat = jsonWpt["lat"].asDouble();
             double lon = jsonWpt["lon"].asDouble();
             double ele = jsonWpt["ele"].asDouble();
             string name = jsonWpt["name"].asString();
             string address = jsonWpt["address"].asString();
             Waypoint *temp = new Waypoint(lat, lon, ele, name, address);
	           //waypoints.insert(name, temp);//won't insert duplicates
	           waypoints[*it] = *temp;
         }
   }
   else {
       std::cout << "Parsing failed!" << endl;
      exit(1); 
   }
}

//Write to a waypoint.json file
void WaypointLibrary::expjson_file() {
    Json::Value jsonLib;
    jsonLib["name"] = name;
    for(std::map<string, Waypoint>::iterator i = waypoints.begin(); i != waypoints.end(); i++) 
    {
        string key = i->first;//gets the key
        Json::Value jsonwpt;
        Waypoint wpt = waypoints[key];
        jsonwpt["lat"] = wpt.lat;
	jsonwpt["lon"] = wpt.lon;
	jsonwpt["ele"] = wpt.ele;
	jsonwpt["name"] = wpt.name;
	jsonwpt["address"] = wpt.address;
	jsonLib[key] = jsonwpt;
    }
    Json::StyledStreamWriter ssw("  ");
    std::ofstream jsonOutFile("waypoint.json", std::ofstream::binary);
    ssw.write(jsonOutFile, jsonLib);
}

int WaypointLibrary::getSize(){
    return waypoints.size();
}

//Calculates the great circle distance between two points
double WaypointLibrary::distance(Waypoint from, Waypoint to) {
    double delVarphi = toRadians(to.lat - from.lat);
    double delLamda = toRadians(to.lon - from.lon);
    double varphiFrm = toRadians(from.lat);
    double varphiTo = toRadians(to.lat);
    double a = sin(delVarphi/2) *sin(delVarphi/2) + cos(varphiFrm) * 
	    cos(varphiTo) * sin(delLamda/2) * sin(delLamda/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return 6371 * c;
}

double WaypointLibrary::toRadians(double input) {
    const double halfC = M_PI * input;
    return halfC / 180; 
}

double WaypointLibrary::toDegrees(double input) {
    return input * 180/M_PI;
}

//Calculates the formula for initial bearing
double WaypointLibrary::bearing(Waypoint from, Waypoint to) {
    double delLamda = toRadians(to.lon - from.lon); 
    double varphiFrm = toRadians(from.lat);
    double varphiTo = toRadians(to.lat);
    double lamdFrm = toRadians(from.lon);
    double lamdTo = toRadians(to.lon);
    double y = sin(lamdTo - lamdFrm) * cos(varphiTo);
    double x = cos(varphiFrm) * sin(varphiTo) - sin(varphiFrm) *
	    cos(varphiTo) * cos(lamdTo - lamdFrm);
    double ret = toDegrees(atan2(y, x));
    ret = fmod((ret + 360), 360);
    return ret;
}
