Author: Tim Lindquist (Tim.Lindquist@asu.edu)
        Software Engineering, CIDSE, IAFSE, Arizona State University Polytechnic
Version: January 2018

See http://pooh.poly.asu.edu/Ser321/Assigns/Assign3/assign6.html

Purpose: Sample Java Client demonstrating the browser client to be used
 in completing Assignment 6 Implementation of HTTP Daemon via Java Threaded TCP/IP
 sockets.

This program is has been tested on Linux and MacOS.

To execute Ant using the build.xml in this directory, you will need to
copy the file: antlibs.jar from the lib directory to your home directory:
cp lib/antlibs.jar ~
or
cp lib/antlibs.jar $HOME/
Note that ~ (tilde) is a shortcut for $HOME
then extract the antlibs.jar file:
pushd ~
jar xvf antlibs.jar
pushd -0
The pushd commands manipulate a stack of directories for switching your
bash's current directory. The first pushd pushes home onto the stack and
switches the current directory to home. The second pushd takes you
back to whatever directory you were in before the first.

To run the example GUI, from a bash shell in the project directory, execute the
command:
ant execute.sample

Note that other targets defined in the build.xml file may not work with this
sample project and are part of the instructors solution to the problem.

To clean the project (remove the .class files) execute:
ant clean
