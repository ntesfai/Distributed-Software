package ser321;

import java.lang.Math;
import java.io.*;
import java.util.*;
import java.text.NumberFormat;
import org.json.JSONObject;


/**
 * Copyright 2018 Tim Lindquist,
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
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *      Software Engineering, CIDSE, IAFSE, Arizona State University Polytechnic
 * @version January, 2018
 **/

public class Waypoint extends Object {

   public final static int STATUTE = 0;
   public final static int NAUTICAL = 1;
   public final static int KMETER = 2;
   public final static double radiusE = 6371;

   public double lat; // degrees lat in DD.D format (+ north, - south)
   public double lon; // degrees lon in DD.D format (+ east, - west)
   public double ele; // elevation in feet MSL
   public String name; // a name for the waypoint
   public String address; //an address for the waypoint

   public Waypoint(double lat, double lon, double ele, String name){
      this.lat = lat;
      this.lon = lon;
      this.ele = ele;
      this.name = name;
      this.address = null;
   }
   
   public Waypoint(JSONObject obj) {
	   this.lat = obj.getDouble("lat");
	   this.lon = obj.getDouble("lon");
	   this.ele = obj.getInt("ele");
	   this.name = obj.getString("name");
	   this.address = obj.getString("address");
   }
   
   public JSONObject toJSONObject() {
	   JSONObject obj = new JSONObject();
	   obj.put("lat", lat);
	   obj.put("lon", lon);
	   obj.put("ele", ele);
	   obj.put("name", name);
	   obj.put("address", address);
	   
	   return obj;
   }
   
   /**
    * calculate the distance great circle route between this and the
    * argument waypoints. Return the result in the requested scale
    */
   public double distanceGCTo(Waypoint wp, int scale){
      double ret = 0.0;
      // ret is in kilometers if you use the availble algorithms. S
      // Switch to either Statute or Nautical if necessary
      switch(scale) {
      case STATUTE:
         ret = ret * 0.62137119;
         break;
      case NAUTICAL:
         ret = ret * 0.5399568;
         break;
      }
      return ret;
   }

   /**
    * calculate the initial heading on the circle route between this and the
    * argument waypoints. Return the result.
    */
   public double bearingGCInitTo(Waypoint wp, int scale){
      double ret = 0.0;
      return ret;
   }

   public void print () {
      System.out.println("Waypoint "+name+": lat "+lat+" lon "+lon+
                         " elevation "+ele);
   }

   public static void main(String args[]) {
      try {
         String url = "http://127.0.0.1:8080/";
         if(args.length > 0){
            url = args[0];
         }
         Vector<Waypoint> points = new Vector<Waypoint>();
         Waypoint wp;
         BufferedReader stdin = new BufferedReader(
            new InputStreamReader(System.in));
         System.out.print("Enter waypoint lat lon ele name or calc>");
         String inStr = stdin.readLine();
         StringTokenizer st = new StringTokenizer(inStr);
         String opn = st.nextToken();
         double lat, lon, ele = 0;
         String name;
         NumberFormat nf = NumberFormat.getInstance();
         nf.setMaximumFractionDigits(2);
         while(!opn.equalsIgnoreCase("calc")) {
            lat = Double.parseDouble(opn);
            lon = Double.parseDouble(st.nextToken());
            ele = Double.parseDouble(st.nextToken());
            name = st.nextToken();
            wp = new Waypoint(lat, lon, ele, name);
            wp.print();
            points.add(wp);
            System.out.print("Enter waypoint lat lon ele name or calc>");
            inStr = stdin.readLine();
            st = new StringTokenizer(inStr);
            opn = st.nextToken();
         }
         Waypoint next, last = null;
         for(Enumeration<Waypoint> e = points.elements();e.hasMoreElements();){
            next = e.nextElement();
            if (last != null) {
               System.out.println("GC distance from "+last.name+" to "+
                                  next.name + " is "+ nf.format(
                                  last.distanceGCTo(next, Waypoint.STATUTE))
                                  + " bearing " + 
                                  nf.format(last.bearingGCInitTo(next,
                                                          Waypoint.STATUTE)));
            }
            last = next;
         }
      }catch (Exception e) {
         System.out.println("Oops, you didn't enter the right stuff");
      }
   }

}
