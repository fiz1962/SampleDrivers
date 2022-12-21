#!/usr/bin/env python3
"""
Very simple HTTP server in python for logging requests
Usage::
    ./server.py [<port>]
"""
from http.server import BaseHTTPRequestHandler, HTTPServer
import logging
import requests
from urllib.parse import urlparse, parse_qs

ext_uuid = ""
HUB_IP   = ""
HUB_PORT = ""

resp =  b'<root xmlns="urn:schemas-upnp-org:device-1-0" configId="1">\r\n' \
        b'<specVersion>\r\n' \
        b'<major>2</major>\r\n' \
        b'<minor>0</minor>\r\n' \
        b'</specVersion>\r\n' \
        b'<device>\r\n' \
        b'<deviceType>urn:SmartThingsCommunity:device:ContactSensor:1</deviceType>\r\n' \
        b'<presentationURL>/</presentationURL>\r\n' \
        b'<friendlyName>ContactSensorESP8266</friendlyName>\r\n' \
        b'<manufacturer>SmartThingsCommunity</manufacturer>\r\n' \
        b'<manufacturerURL>https://community.smartthings.com</manufacturerURL>\r\n' \
        b'<modelName>LAN ContactSensor</modelName>\r\n' \
        b'<serialNumber>SN-ESP8266-827</serialNumber>\r\n' \
        b'<UDN>uuid:9487da-SN-ESP8266-827</UDN>\r\n' \
        b'</device></root>\r\n\r\n'
              


class S(BaseHTTPRequestHandler):
 
    
    def do_GET(self):
        logging.info("GET request Path: %s\n", str(self.path) )
        
        if self.path.startswith('/control'):
             global ext_uuid
             global HUB_IP
             global HUB_PORT
        
             logging.info(ext_uuid)
             logging.info(HUB_IP)
             logging.info(HUB_PORT)
             
             u = urlparse(self.path)
             qs = parse_qs(u.query)

             ref = { \
                  "status":"HTTP/1.1 200 OK",
                  "Content-Type":"application/json",
                  "Cache-Control":"no-cache, private",
                  "contact":qs['contact'][0],
                  "uuid":ext_uuid,
                  }

             self.send_response(200)
             r = requests.post(url='http://' + HUB_IP + ':' + HUB_PORT + '/push-state', json=ref)

        if self.path == '/ContactSensorESP8266.xml':
            logging.info("Sending contact sensor xml\n")

            #self.send_header("Content-type", "text/xml")
            #self.end_headers()
            self.wfile.write(resp)
            self.send_response(200)
 
        if self.path == '/refresh?':
            logging.info("Sending refresh\n")

            ref = '{\r\n' \
                  '"status":"HTTP/1.1 200 OK",\r\n' \
                  '"Content-Type":"application/json",\r\n' \
                  '"Cache-Control":"no-cache, private",\r\n' \
                  '"contact":"open"\r\n' \
                  '}/r/n'

            #self.send_header("Content-type", "text/json")
            #self.end_headers()
            self.wfile.write(bytes(ref, 'utf-8'))
            self.send_response(200)
            

            
    def do_POST(self):
        global ext_uuid
        global HUB_IP
        global HUB_PORT
        
        strPath = str(self.path)
        logging.info("POST request Path: %s\n", strPath)
        u = urlparse(self.path)
        logging.info(u)
        qs = parse_qs(u.query)
        logging.info(qs)
        ext_uuid = qs['ext_uuid'][0]
        HUB_IP   = qs['ip'][0]
        HUB_PORT = qs['port'][0]
        
            
def run(server_class=HTTPServer, handler_class=S, port=8191):
    logging.basicConfig(level=logging.INFO)
    server_address = ('0.0.0.0', port)
    httpd = server_class(server_address, handler_class)
    logging.info('Starting httpd...\n')
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    logging.info('Stopping httpd...\n')

if __name__ == '__main__':
    from sys import argv

    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run()
