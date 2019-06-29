#include "WaypointGUI.cpp"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Multiline_Input.H>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

/**
 * Copyright (c) 2018 Tim Lindquist,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ FLTK client UI for Waypoint management.
 * This class extends the Gui component class WaypointGUI and demonstrates
 * sample control functions that respond to button clicks drop-down selects.
 * This software is meant to run on Linux and MacOS using g++.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu) CIDSE - Software Engineering,
 *                       IAFSE, ASU at the Polytechnic campus
 * @file    samplemain.cpp
 * @date    January, 2018
 **/

class SampleStudentClient : public WaypointGUI {
    std::string path = "waypoint.json";
   /** ClickedX is one of the callbacks for GUI controls.
    * Callbacks need to be static functions. But, static functions
    * cannot directly access instance data. This program uses "userdata"
    * to get around that by passing the instance to the callback
    * function. The callback then accesses whatever GUI control object
    * that it needs for implementing its functionality.
    */
   static void ClickedX(Fl_Widget * w, void * userdata) {
      std::cout << "You clicked Exit" << std::endl;
      exit(1);
   }

   static void ClickedRemoveWP(Fl_Widget * w, void * userdata) {
      SampleStudentClient* anInstance = (SampleStudentClient*)userdata;
      WaypointLibrary * wptLib = anInstance->wpLib;
      Fl_Input_Choice * theWPChoice = anInstance->frWps;
      Fl_Input_Choice * toWpt = anInstance->toWps;
      if(theWPChoice->menubutton()->size() == 1) {
	  std::cout << "Fl_Input is empty..." << endl;
          theWPChoice->value("empty");
	  return;
      }
      std::string selected(theWPChoice->value());
      std::cout << "You clicked the remove waypoint button with "
                << selected
                << std::endl;
      for (int i=0; i < theWPChoice->menubutton()->size(); i++ ) {
         const Fl_Menu_Item &item = theWPChoice->menubutton()->menu()[i];
         if(!selected.compare(item.label())){  // if they are equal
	 cout << "Value is: " << theWPChoice->menubutton()->menu()[i].value()<< endl;
            theWPChoice->menubutton()->remove(i);
	    toWpt->menubutton()->remove(i);
            cout << "removed " << selected << endl;
	    theWPChoice->value("");
      	    string host = "http://127.0.0.1:8080";
      	    HttpClient httpclient(host);
      	    waypointclientstub wcs(httpclient);
	    wcs.removeWpt(selected);
            //wptLib->removeWpt(selected);
            break;
         }
      }
  }

   static void ClickedAddWP(Fl_Widget * w, void * userdata) {
      SampleStudentClient* anInstance = (SampleStudentClient*)userdata;
      Fl_Input_Choice * fromWPChoice = anInstance->frWps;
      Fl_Input_Choice * toWPChoice = anInstance->toWps;
      Fl_Input * theLat = anInstance->latIn;
      Fl_Input * theLon = anInstance->lonIn;
      Fl_Input * theEle = anInstance->eleIn;
      Fl_Input * theName = anInstance->nameIn;
      Fl_Multiline_Input *theAddress = anInstance->addrIn;
      WaypointLibrary * wpLib = anInstance->wpLib;
      std::string lat(theLat->value());
      // what follows is not expedient, but shows how to convert to/from
      // double and formatted C and C++ strings.
      double latNum = atof(lat.c_str());  //convert from string to double
      //char latFormat[10];
      //sprintf(latFormat,"%4.4f",latNum);  //format the double into a C string
      //std::string latCppStr(latFormat);   //convert formatted C str to C++ str
      std::string lon(theLon->value());
      double lonNum = atof(lon.c_str());
      std::string ele(theEle->value());
      double eleNum = atof(ele.c_str());
      std::string name(theName->value());
      /*std::cout << "You clicked the add waypoint button lat: "<< latCppStr
                << " lon: " << lon << " ele: " << ele << " name: "
                << name << std::endl;*/
      std::string address(theAddress->value());
      string host = "http://127.0.0.1:8080";
      HttpClient httpclient(host);
      waypointclientstub wcs(httpclient);
      wcs.addWpt(latNum, lonNum, eleNum, name, address);
      //wpLib->addWpt(latNum, lonNum, eleNum, name, address);
      fromWPChoice->add(name.c_str());
      toWPChoice->add(name.c_str());
      fromWPChoice->value(name.c_str());
   }

   //Display data values to GUI
   static void SelectedFromWP(Fl_Widget * w, void * userdata) {
      SampleStudentClient* anInstance = (SampleStudentClient*)userdata;
      WaypointLibrary * wpLib = anInstance->wpLib;
      Fl_Input *latIn = anInstance->latIn;
      Fl_Input *lonIn= anInstance->lonIn;
      Fl_Input *eleIn= anInstance->eleIn;
      Fl_Input *nameIn= anInstance->nameIn;
      Fl_Input_Choice * frWps = anInstance->frWps;
      Fl_Multiline_Input *addrIn = anInstance->addrIn;
      std::string selected(frWps->value());
      std::cout << "You changed the selection in the Fl_Input_Choice from to the value "
                << selected
                << std::endl;
      /*for(int i=0; i < frWps->menubutton()->size(); i++) {
          const Fl_Menu_Item &item = frWps->menubutton()->menu()[i];
	  if(!selected.compare(item.label())) {
	      frWps->value(frWps->menubutton()->menu()[i].label()); 
	      std::cout << "changed Fl_Input widge to " << selected << endl;
	      break;
	  }*/
      char format[10];
      sprintf(format, "%5.3f", wpLib->waypoints[selected].lat);
      latIn->value(format);
      sprintf(format, "%5.3f", wpLib->waypoints[selected].lon);
      lonIn->value(format);
      sprintf(format, "%5.3f", wpLib->waypoints[selected].ele);
      eleIn->value(format);
      nameIn->value(wpLib->waypoints[selected].name.c_str());
      addrIn->value(wpLib->waypoints[selected].address.c_str());
   }
   
   static void SelectedToWp(Fl_Widget * w, void * userdata) {
      SampleStudentClient* anInstance = (SampleStudentClient*)userdata;
      WaypointLibrary * wpLib = anInstance->wpLib;
      Fl_Input *latIn = anInstance->latIn;
      Fl_Input *lonIn= anInstance->lonIn;
      Fl_Input *eleIn= anInstance->eleIn;
      Fl_Input *nameIn= anInstance->nameIn;
      Fl_Input_Choice * toWps= anInstance->toWps;
      Fl_Multiline_Input *addrIn = anInstance->addrIn;
      std::string selected(toWps->value());
      std::cout << "You changed the selection in the Fl_Input_Choice from to the value "
                << selected
                << std::endl;
      /*for(int i=0; i < frWps->menubutton()->size(); i++) {
          const Fl_Menu_Item &item = frWps->menubutton()->menu()[i];
	  if(!selected.compare(item.label())) {
	      frWps->value(frWps->menubutton()->menu()[i].label()); 
	      std::cout << "changed Fl_Input widge to " << selected << endl;
	      break;
	  }*/
      char format[10];
      sprintf(format, "%5.3f", wpLib->waypoints[selected].lat);
      latIn->value(format);
      sprintf(format, "%5.3f", wpLib->waypoints[selected].lon);
      lonIn->value(format);
      sprintf(format, "%5.3f", wpLib->waypoints[selected].ele);
      eleIn->value(format);
      nameIn->value(wpLib->waypoints[selected].name.c_str());
      addrIn->value(wpLib->waypoints[selected].address.c_str());
   }

   //export the waypoints that exist in the waypoint collection
   static void selectedExport(Fl_Widget *w, void* userdata) {
       SampleStudentClient* anInstance = (SampleStudentClient*)userdata; 
       WaypointLibrary* wpts = anInstance->wpLib;
       //wpts->expjson_file();
       string host = "http://127.0.0.1:8080";
       HttpClient httpclient(host);
       waypointclientstub wcs(httpclient);
       wcs.expjson_file();
   }

   //import the waypoint.json library refresh the elements in the fields
   static void selectedImport(Fl_Widget *w, void* userdata) {
       SampleStudentClient* anInstance = (SampleStudentClient*)userdata;
       Fl_Input_Choice* fromW = anInstance->frWps;
       Fl_Input_Choice* toW = anInstance->toWps;
       WaypointLibrary *wpts = anInstance->wpLib; 
       string host = "http://127.0.0.1:8080";
       HttpClient httpclient(host);
       waypointclientstub wcs(httpclient);
       Json::Value root = wcs.impjson_file(); //get all the waypoints
       Json::Value::Members mbrs = root.getMemberNames();
       for(vector<string>::const_iterator it = mbrs.begin(); it != mbrs.end(); it++){
            Json::Value jsonWpt = root[*it]; //get value from root
	    string nameStr = "name";
	    double lat = jsonWpt["lat"].asDouble();
	    double lon = jsonWpt["lon"].asDouble();
	    double ele = jsonWpt["ele"].asDouble();
	    string name = jsonWpt["name"].asString();
	    string address = jsonWpt["address"].asString();
	    Waypoint *temp = new Waypoint(lat, lon, ele, name, address);
	    wpts -> waypoints[*it] = *temp;

        } 

       fromW->clear();
       toW->clear();
       for(std::map<string, Waypoint>::iterator it = wpts->waypoints.begin(); it != wpts->waypoints.end(); it++) {
          string wpName = it->first;
	  fromW->menubutton()->add(wpName.c_str());
	  toW->menubutton()->add(wpName.c_str());
       }
   }

   static void clickedModButt(Fl_Widget *w, void* userdata) {
       SampleStudentClient* anInstance = (SampleStudentClient*)userdata; 
       WaypointLibrary * wptLib = anInstance->wpLib;
       Fl_Input_Choice * toWpt = anInstance->toWps;
       Fl_Input_Choice* froWpt = anInstance->frWps;
       Fl_Input *latIn = anInstance->latIn;
       Fl_Input *lonIn= anInstance->lonIn;
       Fl_Input *eleIn= anInstance->eleIn;
       Fl_Input *nameIn= anInstance->nameIn;
       Fl_Input_Choice * toWps= anInstance->toWps;
       Fl_Multiline_Input *addrIn = anInstance->addrIn;
       string lon(lonIn->value());
       string lat(latIn->value());
       string ele(eleIn->value());
       string name(froWpt->value());
       string nuName(nameIn->value());
       string address(addrIn->value());
       double latNum = atof(lat.c_str());
       double lonNum = atof(lon.c_str());
       double eleNum = atof(ele.c_str());
       string host = "http://127.0.0.1:8080";
       HttpClient httpclient(host);
       waypointclientstub wcs(httpclient);
       Json::Value root = wcs.modWpt(latNum, lonNum, eleNum, nuName, name, address);
       latNum = root["lat"].asDouble();
       lon = root["lon"].asDouble();
       eleNum = root ["ele"].asDouble();
       nuName = root["name"].asString();
       address = root["address"].asString();
       Waypoint *wpt = new Waypoint(latNum, lonNum, eleNum, nuName, address);
       wptLib-> waypoints.insert(std::pair<string, Waypoint>(nuName, *wpt));
       froWpt->value(nuName.c_str());

       for(int i = 0; i < froWpt->menubutton()->size(); i++) {
	   const Fl_Menu_Item &item = froWpt->menubutton()->menu()[i];
           if(!name.compare(item.label())) {
	      froWpt->menubutton()->remove(i);
	      toWpt->menubutton()->remove(i);
	      froWpt->add(nuName.c_str());
	      toWpt->add(nuName.c_str());
	      break;
	   }
       }
   } 

   //clicked on the distBearing button
   static void clickedDistBear(Fl_Widget *w, void* userdata) {
       SampleStudentClient * anInstance = (SampleStudentClient*)userdata;
       WaypointLibrary * wpLib = anInstance->wpLib;
       Fl_Input_Choice * frWpt = anInstance->frWps;
       Fl_Input_Choice * toWpt = anInstance->toWps;
       Fl_Input * dist_bear = anInstance->distBearIn;
       string fromStr(frWpt->value());
       string toStr(toWpt->value());
       double bearing = WaypointLibrary::bearing(wpLib->waypoints[fromStr], wpLib->waypoints[toStr]);
       double dist = WaypointLibrary::distance(wpLib->waypoints[fromStr], wpLib->waypoints[toStr]);
       char format[10];
       char format2[10];
       sprintf(format, "%5.3f", bearing);
       sprintf(format2, "%5.3f", dist);
       //cout << "Error here..." << endl;
       string temp(format);
       string output(format2);
       output.append(" ");
       output.append(temp);
       dist_bear->value(output.c_str());
   }

public:
   SampleStudentClient(const char * name = 0) : WaypointGUI(name) {
      removeWPButt->callback(ClickedRemoveWP, (void*)this);
      addWPButt->callback(ClickedAddWP, (void*)this);
      toWps->callback(SelectedToWp, (void*)this);
      frWps->callback(SelectedFromWP, (void*)this);
      callback(ClickedX);
      modWPButt->callback(clickedModButt, (void*)this);
      importButt->callback(selectedImport, (void*)this);
      exportButt ->callback(selectedExport, (void*)this);
      distBearButt->callback(clickedDistBear, (void*)this);
   }
};

int main() {
   SampleStudentClient cm("Non-Distributed Waypoint Browser");
   return (Fl::run());
}
