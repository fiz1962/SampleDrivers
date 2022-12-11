import socket
import struct
import sys

multicast_group = '239.255.255.250'
server_address = ('', 1900)

# Create the socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind to the server address
sock.bind(server_address)

# Tell the operating system to add the socket to the multicast group
# on all interfaces.
group = socket.inet_aton(multicast_group)
mreq = struct.pack("4sL", group, socket.INADDR_ANY)
sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

# Receive/respond loop
while True:
    #print('\nwaiting to receive message')
    data, address = sock.recvfrom(512)

    #print('received %s bytes from %s' % (len(data), address))
    #print(data)
    if data.decode().find('LightBulbESP8266:1') != -1:
        print("Found light");
        print(address);
        print(data)
        resp = b'HTTP/1.1 200 OK\r\nCache-Control: max-age=100\r\nEXT: SERVER: NodeMCU/Lua5.1.4 UPnP/1.1\r\nLightBulbESP82660.1\r\nST: upnp:rootdevice\r\nUSN: \r\nuuid:ESP8266-1234\r\nLocation: http://192.168.1.223:8190/LightBulbESP8266.xml\r\n\r\n'
        sock.sendto(resp, address)
        quit()

