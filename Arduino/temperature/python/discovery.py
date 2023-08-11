#!/usr/bin/env python3

import socket
import struct
import sys

multicast_group = '239.255.255.250'
server_address = ('', 1900)
devType = bytes('TemperatureESP8266', 'utf-8')
uuid = bytes('ESP8266-1234', 'utf-8')
devPort = bytes('8191', 'utf-8')

resp =  b'<root xmlns="urn:schemas-upnp-org:device-1-0" configId="1">\r\n' \
        b'<specVersion>\r\n' \
        b'<major>2</major>\r\n' \
        b'<minor>0</minor>\r\n' \
        b'</specVersion>\r\n' \
        b'<device>\r\n' \
        b'<deviceType>urn:SmartThingsCommunity:device:TemperatureESP8266:1</deviceType>\r\n' \
        b'<presentationURL>/</presentationURL>\r\n' \
        b'<friendlyName>'+devType+b'</friendlyName>\r\n' \
        b'<manufacturer>SmartThingsCommunity</manufacturer>\r\n' \
        b'<manufacturerURL>https://community.smartthings.com</manufacturerURL>\r\n' \
        b'<modelName>LAN TemperatureSensor</modelName>\r\n' \
        b'<serialNumber>SN-ESP8266-827</serialNumber>\r\n' \
        b'<UDN>uuid:'+uuid+b'</UDN>\r\n' \
        b'</device></root>\r\n\r\n'
              
ext_uuid = ""
HUB_IP   = ""
HUB_PORT = ""

print("Starting sockets") 
# Create the socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock2.connect(("8.8.8.8", 80))
localIP = sock2.getsockname()[0]
sock2.close()
ipBytes = bytes(localIP, 'utf-8')

# Bind to the server address
sock.bind(server_address)

# Tell the operating system to add the socket to the multicast group
# on all interfaces.
group = socket.inet_aton(multicast_group)
mreq = struct.pack("4sL", group, socket.INADDR_ANY)
sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)
print("Starting loop")

file1 = open("MyFile.txt", "w")
# Receive/respond loop
while True:
    #print('\nwaiting to receive message')
    data, address = sock.recvfrom(512)

    #print('received %s bytes from %s' % (len(data), address))
    print(data)
    file1.write(data.decode())
    if data.decode().find(devType.decode()) != -1:
        print("Found temperature");
        print(address);
        print(data)
        resp = b'HTTP/1.1 200 OK\r\nCache-Control: max-age=100\r\nEXT: SERVER: NodeMCU/Lua5.1.4 UPnP/1.1\r\n'+devType+b':1\r\nST: upnp:rootdevice\r\nUSN: \r\nuuid:'+uuid+b'\r\nLocation: http://'+ipBytes+b':'+devPort+b'/'+devType+b'.xml\r\n\r\n'
        sock.sendto(resp, address)
        print(resp)
        file1.close()
        quit()
        

