package ser321.assign;
import java.io.*;
import java.net.*;
import java.util.*;
import org.json.*;
import java.net.URL;
import java.net.HttpURLConnection;
import java.util.zip.GZIPInputStream;

public class WaypointCollectionHttpProxy {
    static final boolean debugOn = false;
    private final Map<String, String> headers;
    private URL url;
    private String requestData;
    private static int callid = 0;

    public WaypointCollectionHttpProxy(URL url){
       this.url = url;
      this.headers = new HashMap<String, String>(); 
    }

   private JSONObject buildCall(String method){
      JSONObject jobj = new JSONObject("{\"jsonrpc\":\"2.0\",\"method\":\""+method+"\",\"id\":"+callid+"}");
      return jobj;
   }

    //Methods recognized by the server
   /*
    *saveToJsonFile()-->boolean
    */
   public boolean expjson_file(){
      boolean ret = false;
      try{
         JSONObject jobj = this.buildCall("expjson_file");
         JSONArray params = new JSONArray();
         jobj.put("params",params);
         String request = jobj.toString();
         String response = this.call(request);
         debug("expjson_file returned: "+response);
         JSONObject respObj = new JSONObject(response);
         if(!respObj.has("error")){
            ret = respObj.optBoolean("result");
         }
      }catch(Exception ex){
         System.out.println("exception in resetFromJsonFile error: "+ex.getMessage());
      }
      return ret;
   }

   public JSONObject impjson_file(){
      JSONObject ret = null;
      try{
         JSONObject jobj = this.buildCall("impjson_file");
         JSONArray params = new JSONArray();
         jobj.put("params",params);
         String request = jobj.toString();
         String response = this.call(request);
         debug("impjson_file returned: "+response);
         JSONObject respObj = new JSONObject(response);
         if(!respObj.has("error")){
            ret = respObj.optJSONObject("result");
         }
      }catch(Exception ex){
         System.out.println("exception in resetFromJsonFile error: "+ex.getMessage());
      }
      return ret;
   }

   public boolean addWpt(double lat, double lon, double ele, String name, String address){
      boolean ret = false;
      try{
         JSONObject jobj = this.buildCall("expjson_file");
         JSONArray params = new JSONArray();
	 params.put(lat);
	 params.put(lon);
	 params.put(ele);
	 params.put(name);
	 params.put(address);
         jobj.put("params",params);
         String request = jobj.toString();
         String response = this.call(request);
         debug("addWpt returned: "+response);
         JSONObject respObj = new JSONObject(response);
         if(!respObj.has("error")){
            ret = respObj.optBoolean("result");
         }
      }catch(Exception ex){
         System.out.println("exception in resetFromJsonFile error: "+ex.getMessage());
      }
      return ret;
   }

   public boolean removeWpt(String name){
      boolean ret = false;
      try{
         JSONObject jobj = this.buildCall("removeWpt");
         JSONArray params = new JSONArray();
         params.put(name);
         jobj.put("params",params);
         String request = jobj.toString();
         String response = this.call(request);
         debug("removeWpt returned: "+response);
         JSONObject respObj = new JSONObject(response);
         if(!respObj.has("error")){
            ret = respObj.optBoolean("result");
         }
      }catch(Exception ex){
         System.out.println("exception in resetFromJsonFile error: "+ex.getMessage());
      }
      return ret;
   }

   public JSONObject modWpt(double lat, double lon, double ele, String nuName, String name, String address){
      JSONObject ret = new JSONObject();
      try{
         JSONObject jobj = this.buildCall("modWpt");
         JSONArray params = new JSONArray();
	 params.put(lat);
	 params.put(lon);
	 params.put(ele);
	 params.put(nuName);
	 params.put(name);
	 params.put(address);
         jobj.put("params",params);
         String request = jobj.toString();
         String response = this.call(request);
         debug("modWpt returned: "+response);
         JSONObject respObj = new JSONObject(response);
         if(!respObj.has("error")){
            ret = respObj.optJSONObject("result");
         }
      }catch(Exception ex){
         System.out.println("exception in resetFromJsonFile error: "+ex.getMessage());
         System.exit(1);
      }
      return ret;
   }

   public int get_size(){
      int ret = 0;
      try{
         JSONObject jobj = this.buildCall("removeWpt");
         JSONArray params = new JSONArray();
         jobj.put("params",params);
         String request = jobj.toString();
         String response = this.call(request);
         debug("get_size returned: "+response);
         JSONObject respObj = new JSONObject(response);
         if(!respObj.has("error")){
            ret = respObj.optInt("result");
         }
      }catch(Exception ex){
         System.out.println("exception in resetFromJsonFile error: "+ex.getMessage());
      }
      return ret;
   }

   public void setHeader(String key, String value) {
      this.headers.put(key, value);
   }

   public String call(String requestData) throws Exception {
      debug("in call, url: "+url.toString()+" requestData: "+requestData);
      String respData = post(url, headers, requestData);
      return respData;
   }

   private String post(URL url, Map<String, String> headers, String data) throws Exception {
      HttpURLConnection connection = (HttpURLConnection) url.openConnection();
      this.requestData = data;
      if (headers != null) {
         for (Map.Entry<String, String> entry : headers.entrySet()) {
            connection.addRequestProperty(entry.getKey(), entry.getValue());
         }
      }
      connection.addRequestProperty("Accept-Encoding", "gzip");
      connection.setRequestMethod("POST");
      connection.setDoOutput(true);
      connection.connect();
      OutputStream out = null;
      try {
         out = connection.getOutputStream();
         out.write(data.getBytes());
         out.flush();
         out.close();
         int statusCode = connection.getResponseCode();
         if (statusCode != HttpURLConnection.HTTP_OK) {
            throw new Exception(
               "Unexpected status from post: " + statusCode);
         }
      } finally {
         if (out != null) {
            out.close();
         }
      }
      String responseEncoding = connection.getHeaderField("Content-Encoding");
      responseEncoding = (responseEncoding == null ? "" : responseEncoding.trim());
      ByteArrayOutputStream bos = new ByteArrayOutputStream();
      InputStream in = connection.getInputStream();
      try {
         in = connection.getInputStream();
         if ("gzip".equalsIgnoreCase(responseEncoding)) {
            in = new GZIPInputStream(in);
         }
         in = new BufferedInputStream(in);
         byte[] buff = new byte[1024];
         int n;
         while ((n = in.read(buff)) > 0) {
            bos.write(buff, 0, n);
         }
         bos.flush();
         bos.close();
      } finally {
         if (in != null) {
            in.close();
         }
      }
      debug("post: json rpc request via http returned string "+bos.toString());
      return bos.toString();
   }

   private static void debug(String message) {
      if (debugOn)
         System.out.println("debug: "+message);
   }

}

