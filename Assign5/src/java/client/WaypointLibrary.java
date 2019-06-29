package ser321;

import org.json.JSONObject;
import org.json.JSONString;
import org.json.JSONTokener;
import java.io.File;
import java.io.FileInputStream;
import java.io.PrintWriter;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.Vector;
import java.util.Arrays;
import java.util.Enumeration;

/*Class that reads the .json file and creates and maintains
 *a vector of Waypoint instances*/
/**
 * Copyright 2018 Nubian Tesfai,
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Class to perform various earth surface calculations. Given lat/lon points
 * this class can perform distance and bearing calculations - Great Circle,
 * and Rhumb-line.
 *
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Nubian Tesfai ntesfai@asu.edu
 *      Software Engineering, CIDSE, IAFSE, Arizona State University Polytechnic
 * @version January, 2018
 **/
public class WaypointLibrary implements JSONString {
    public Vector<Waypoint> wpts;
    public JSONObject obj;
    public WaypointLibrary() {
    	try {
	    	FileInputStream inFile = new FileInputStream("waypoints.json");
	    	obj = new JSONObject(new JSONTokener(inFile));
	    	String names[] = JSONObject.getNames(obj);
	    	System.out.println("Names are: "+ Arrays.toString(names));
	    	wpts = new Vector<Waypoint>();
	    	for(int i = 0; i < names.length; i++) {
	    		Waypoint temp = new Waypoint((JSONObject)obj.getJSONObject(names[i]));
	    		wpts.add(temp);
	    	}
	    }
    	catch(Exception ex) {
    		System.out.println("Exception importing JSON lib: "+ ex.getMessage());
    	}
    }
    
    //Method to add additional waypoints when user clicks Add Waypoint to both the collection and the library
    public boolean addWpt(double lat, double lon, double ele, String name, String addr) {
    	Waypoint wpt = new Waypoint(lat, lon, ele, name);
    	wpt.address = addr;
    	obj.put(name, wpt.toJSONObject());
    	return wpts.add(wpt);

    }
    
    /*Remove the selected waypoint selected from frWps from the library*/
    public boolean rmvWpt(String key) {
    	boolean found = false;
    	for(int i = 0; i < wpts.size(); i++) {
    		if(wpts.elementAt(i).name.equals(key)) {
			System.out.println("Removed: " + wpts.remove(i).name + " from collection");
    			System.out.println("Removed: " + obj.remove(key) + " from library");
    		}
		return true;
    	}
        return false;
    }
    
    //Method to modify an existing waypoint when user clicks Modify waypoint
    //Throws error if Waypoint not in the Vector
    public JSONObject modLib(double lat, double lon, double ele, String name, String addr)    {
    	boolean found = false;
	JSONObject ret = null;
    	Waypoint wpt = new Waypoint(lat, lon, ele, name);
    	wpt.address = addr;
	int indx = 0;
    	for(int i = 0; i < wpts.capacity(); i++) {
    		if(wpts.elementAt(i).name.equals(name)) {
    			found = true;
			indx = i;
    			break;
    		}
    	}
    	if(found) {
    	    wpts.removeElementAt(indx);
    	    obj.remove(name);
    	    wpts.add(wpt);
    	    obj.put(name, wpt.toJSONObject());
            ret = obj.getJSONObject(name);
    	}
	else if(!found){
	    System.out.println("Element not found");
	    System.exit(1);
	}
	return ret;
    }
    
    //Method to write to the waypoints.json lib when user clicks Export Library
    public boolean exptLib() {
    	try {
    	PrintWriter pwrt = new PrintWriter("waypoints.json");
    	pwrt.println(toJSONString());
    	pwrt.close();
	return true;
    	}
    	catch(Exception ex) {
    		ex.printStackTrace();
    	}
	return false;
    }
    
    /*A method that imports an existing json library and updates the Waypoint collection*/
    public JSONObject imptLib() {
    	wpts.removeAllElements(); //Don't want any duplicate elements
    	try {
    		FileInputStream inStream = new FileInputStream("waypoints.json");
    		obj = new JSONObject(new JSONTokener(inStream));
    		String[] names = JSONObject.getNames(obj);
    		for(int i = 0; i < names.length; i++) {
    			Waypoint wp = new Waypoint((JSONObject)obj.getJSONObject(names[i]));
    			wpts.add(wp);
    		}
    	}
    	catch(Exception ex) {
    		ex.printStackTrace();
		System.exit(1);
    	}
	if(obj.equals(null))
	    System.exit(1);

        return obj;
    }
    
    /*Method returns a string of a JSONObject of all objects in the data structure*/
    public String toJSONString() {
    	String ret;
    	JSONObject obj = new JSONObject();
    	for(Enumeration<Waypoint> e = wpts.elements(); e.hasMoreElements();) {
    		Waypoint temp = (Waypoint)e.nextElement();
    		obj.put(temp.name, temp.toJSONObject());
    	}
    	ret = obj.toString();
    	return ret;
    }
}
