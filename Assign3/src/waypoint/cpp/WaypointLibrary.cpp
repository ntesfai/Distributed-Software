#include "WaypointLibrary.hpp"
#include <iostream>
#include <json/json.h>
#include <fstream>

/*Create an instance of the WaypointLibrary and initialize it*/

WaypointLibrary::WaypointLibrary() {}

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

void WaypointLibrary::removeWpt(string wpt) {
    waypoints.erase(wpt); 
}

void WaypointLibrary::addWpt(double lat, double lon, double ele, string name, string address) {
    Waypoint *temp = new Waypoint(lat, lon, ele, name, address);
    //waypoints[name] = *temp;
    waypoints.insert(std::pair<string, Waypoint>(name, *temp));
}

void WaypointLibrary::modWpt(double lat, double lon, double ele, string name, string address) {
    if(waypoints.count(name) > 0){
        Waypoint *temp = new Waypoint(lat, lon, ele, name, address);
	waypoints[name] = *temp;
    }
}

WaypointLibrary WaypointLibrary::impjson_file() {
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
       return waypoints;
   }
}

void WaypointLibrary::expjson_file() {
    Json::Value jsonLib;
    for(std::map<string, User>::iterator i = users.begin(); i !=end(); i++) 
    {
        string key = i->first;//gets the key
        Json::Value jsonwpt;
        Waypoint wpt = waypoints[key];
        jsonwpt["lat"] = wpt.lat;
	jsonwpt["lon"] = wpt.lon;
	jsonwpt["ele"] = wpt.ele;
	jsonwpt["name"] = wpt.name;
	jsonwpt["address"] = wpt.address;
	jsonLib
    }
}
