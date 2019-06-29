package ser321.http.server;

import java.net.*;
import java.io.*;
import java.lang.StringBuilder;
import java.util.*;
import ser321.http.client.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;

/**
 * Copyright 2015 Tim Lindquist,
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
 * A class for client-server connections with a threaded server.
 * The echo server creates a server socket. Once a client arrives, a new
 * thread is created to service all client requests for the connection.
 * The example includes a java client and a C# client. If C# weren't involved,
 * the server and client could use a bufferedreader, which allows readln to be
 * used, and printwriter, which allows println to be used. These avoid
 * playing with byte arrays and encodings. See the Java Tutorial for an
 * example using buffered reader and printwriter.
 *
 * Ser321 Foundations of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version August 2015
 */
/**
 * @author Tim Lindquist ASU Polytechnic Department of Engineering
 * @version October 2009
 */
public class ThreadedEchoServer extends Thread implements Runnable {
  private Socket conn;
  private int id;
  private SimpleDateFormat simpleDate;
  private int callCount = 1;
/*private final String HTTP_OK = "ok";
  private final String HTTP_ERR = "404";
  private final String OKresponse = new String("HTTP/1.1 200 OK\n\r");
  private final String nFoundResponse = new String("HTTP/1.1 400 Not Found\n\r");
  private String contentType = "";
  private String contentSize = "";
  private String fileContents = "";
*/
  public ThreadedEchoServer (Socket sock, int id) {
    this.conn = sock;
    this.id = id;
	this.simpleDate = new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss z");
  }

	//Method that returns the contents of a file as a String
	public String readFile(String path) throws IOException {
	  StringBuilder text = new StringBuilder("<html>");
	  try{
	  //byte[] encoded = Files.readAllBytes(Paths.get(path));
	  //return new String(encoded);
	  //if(path == "")
	      //path = "www/Ser321/index.html";
	  String temp = null;
	  File file = new File(path);
	  BufferedReader bufRead = new BufferedReader(new FileReader(file));
	  temp = bufRead.readLine();
	  while((temp = bufRead.readLine()) != null) {
          text.append(temp);
   	      //System.out.println(text);
      }
	  }catch(FileNotFoundException e){e.printStackTrace();}
	   return text.toString();
	}

   //Method for parsing the request from the client
   public String parseHTTP(String clientString){
		
		//System.out.println(clientString);
		StringBuilder pathStr = new StringBuilder();
	 	pathStr.append(clientString.toCharArray(), 
   	    clientString.indexOf("www"), 
		clientString.indexOf("HTTP") - 6);
	   
        return pathStr.toString();
    }


   public void run() {
      try {
		 final String OKresponse = new String("HTTP/1.1 200 OK");
		 final String failResponse = new String("HTTP/1.1 404 Not Found");
		 String clientString = "";
		 String contentType = "";
		 String fileContents = "";
		 String pathStr = null;
		 StringBuilder contentSize = new StringBuilder("Content-Length: ");
         StringBuilder response = new StringBuilder();
		 StringBuilder clientStrBuild = new StringBuilder();
         OutputStream outSock = conn.getOutputStream();
         InputStream inSock = conn.getInputStream();
		 String date = simpleDate.format(new Date());
		 //BufferedReader bufRead = new BufferedReader(new InputStreamReader(inSock));
		 //BufferedWriter bufWrite = new BufferedWriter(new OutputStreamWriter(outSock));

		//Read in the HTTP request
		byte clientInput[] = new byte[32000];
		int numr = inSock.read(clientInput, 0, clientInput.length -1 );
		try{
			if(numr != -1) {
		        String requestString = new String(clientInput, 0, clientInput.length - 1);
				clientStrBuild.append(requestString);
				System.out.println(requestString);
				//numr = inSock.read(clientInput, 0, 65352);
		    }
		}catch(Exception ex){ex.printStackTrace();}
        System.out.println("Testing");

		 //String temp = bufRead.readLine(); //FileNotFound is thrown **fixed**

	     /*try{	
			 while(temp != null) {
				 clientStrBuild.append(temp + "\r\n");
				 temp = bufRead.readLine();
			     System.out.println("Testing3");
			 }
		 }catch(Exception ex){ex.printStackTrace();}
		 */

		clientString = clientStrBuild.toString();
		System.out.println("HTTP Request:" + clientString);
		//Check the type of HTTP request we have: GET, POST, DELETE, PUT, HEAD
        if(clientString.contains("GET") && clientString.contains("HTTP/1.1")) {
			if(clientString.contains(".html")) {
                System.out.println("Testing2");
  			    pathStr = parseHTTP(clientString);
				contentType = "Content-Type: text/html";
			}
		}
		
		//Resource not found send 404 response
		else{
        	response.append(failResponse + "\r\n");
			response.append("Date: " + date + "\r\n");
			outSock.write(response.toString().getBytes());
			inSock.close();
			outSock.close();
			conn.close();
			return;
        }

		 //print the path to the resource and get file contents
		 System.out.println("Path: " + pathStr);
		 fileContents = readFile(pathStr);
		 //System.out.println(fileContents);

		 contentSize.append(String.valueOf( fileContents.length()));
		 response.append(OKresponse + "\r\n");
		 response.append("Date: " + date + "\r\n");
		 response.append(contentType + "\r\n");
		 response.append(contentSize + "\r\n" + "\n");
		 response.append(fileContents + "\r\n");
		 System.out.println(response.toString());

		 if(fileContents != null) {
			 //Write the contents along with the server's response (code and html file contents)
		     /**bufWrite.write(OKresponse, 0, OKresponse.length());
			 bufWrite.newLine();
	   		 bufWrite.write(simpleDate.toPattern(), 0, simpleDate.toPattern().length());
			 bufWrite.newLine();
			 bufWrite.write(contentType, 0, contentType.length());
			 bufWrite.newLine();
			 bufWrite.write(contentSize, 0, contentSize.length());
			 bufWrite.newLine();
			 bufWrite.newLine();
			 bufWrite.write(fileContents, 0, fileContents.length());
			 */
		     outSock.write(response.toString().getBytes());
			 inSock.close();
			 outSock.close();
			 conn.close();
             System.out.println("Testing5");
		 }
		 }catch (IOException e) {
		  e.printStackTrace();
          System.out.println("Can't get I/O for the connection. " + e);
          }
   }
    
   public static void main (String args[]) {
    Socket sock;
    int id=0;
    try {
      if (args.length != 1) {
        System.out.println("Usage: java ser321.sockets.ThreadedEchoServer"+
                           " [portNum]");
        System.exit(0);
      }

	  //System.out.println(System.getProperty("user.dir"));
      int portNo = Integer.parseInt(args[0]);
      if (portNo <= 1024) portNo=8080;
      ServerSocket serv = new ServerSocket(portNo);
      while (true) {
        System.out.println("Echo server waiting for connects on port "
                            +portNo);
        sock = serv.accept();
        System.out.println("Echo server connected to client: "+id);
        ThreadedEchoServer myServerThread = new ThreadedEchoServer(sock,id++);
        //String file = myServerThread.readFile("www/Ser321/index.html");
		//System.out.println(file);
        myServerThread.start();
      }
    } catch(Exception e) {e.printStackTrace();}
  }
}
