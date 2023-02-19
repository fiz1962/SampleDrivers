
multicast_group = '239.255.255.250'
server_address = ('', 1900)
devType = bytes('TemperatureESP8266', 'utf-8')
uuid = bytes('ESP8266-1234', 'utf-8')
devPort = bytes('8192', 'utf-8')

resp =  b'<root xmlns="urn:schemas-upnp-org:device-1-0" configId="1">\r\n' \
        b'<specVersion>\r\n' \
        b'<major>2</major>\r\n' \
        b'<minor>0</minor>\r\n' \
        b'</specVersion>\r\n' \
        b'<device>\r\n' \
        b'<deviceType>urn:SmartThingsCommunity:device:Temperature:1</deviceType>\r\n' \
        b'<presentationURL>/</presentationURL>\r\n' \
        b'<friendlyName>'+devType+b'</friendlyName>\r\n' \
        b'<manufacturer>SmartThingsCommunity</manufacturer>\r\n' \
        b'<manufacturerURL>https://community.smartthings.com</manufacturerURL>\r\n' \
        b'<modelName>LAN Temperature</modelName>\r\n' \
        b'<serialNumber>SN-ESP8266-897</serialNumber>\r\n' \
        b'<UDN>uuid:'+uuid+b'</UDN>\r\n' \
        b'</device></root>\r\n\r\n'
              
ext_uuid = ""
HUB_IP   = ""
HUB_PORT = ""

