# Connecting to the UTA RTK base station in Mission Planner

The UTA RTK base station broadcasts RTCM correction data using RTK2GO.com. An NTRIP client can connect to the base station using the appropriate connection string, which generally includes username, password, port, and endpoint fields. For NTRIP connections in Mission Planner using RTK2GO.com, the connection strings have the following format:

## NTRIP connection format
user:password@host:port/mountport

## NTRIP connection fields
user - a valid email address (your email address)

password - the string "none"

host - rtk2go.com

port - 2101

mountpoint - name of the NTRIP caster you with to connect to


In the case of the UTA RTK base station mounted on the roof of Nedderman Hall, the mountpoint is "ARLINGTON_76010". Thus, the proper connection string is:

## UTA Mission Planner NTRIP connection string
http://email@domain.com:none@rtk2go.com:2101/ARLINGTON_76010

## To enter the NTRIP connection information

navigate to SETUP > Optional Hardware > RTK/GPS Inject

![Mission Planner NTRIP setup](https://github.com/cmcmurrough/cse4316/blob/master/RTK/mission_planner_NTRIP_setup.png?raw=true)

Select "NTRIP" and click "Connect". Enter the connection string in the popup after replacing the email address with your own.

![Mission Planner connection](https://github.com/cmcmurrough/cse4316/blob/master/RTK/mission_planner_connection.png?raw=true)

After clicking "OK", the client should connect and show link status information as packets are received.

# Checking base station status
Real-time status information of the UTA base stations can be checked from a browser:

http://rtk2go.com:2101/SNIP::MOUNTPT?baseName=ARLINGTON_76010

# View other active streams on RTK2GO
A list of all active RTK2GO base stations can be found here:

http://new.rtk2go.com:2101/SNIP::STATUS
