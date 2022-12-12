#!/usr/bin/env python3
"""
Very simple HTTP server in python for logging requests
Usage::
    ./server.py [<port>]
"""
from http.server import BaseHTTPRequestHandler, HTTPServer
import logging

resp =  b'<root xmlns="urn:schemas-upnp-org:device-1-0" configId="1">\r\n' \
        b'<specVersion>\r\n' \
        b'<major>2</major>\r\n' \
        b'<minor>0</minor>\r\n' \
        b'</specVersion>\r\n' \
        b'<device>\r\n' \
        b'<deviceType>urn:SmartThingsCommunity:device:Light:1</deviceType>\r\n' \
        b'<presentationURL>/</presentationURL>\r\n' \
        b'<friendlyName>LightBulbESP8266</friendlyName>\r\n' \
        b'<manufacturer>SmartThingsCommunity</manufacturer>\r\n' \
        b'<manufacturerURL>https://community.smartthings.com</manufacturerURL>\r\n' \
        b'<modelName>RGB LightBulb</modelName>\r\n' \
        b'<serialNumber>SN-ESP8266-897</serialNumber>\r\n' \
        b'<UDN>uuid:9487da-SN-ESP8266-897</UDN>\r\n' \
        b'</device></root>\r\n\r\n'
              


class S(BaseHTTPRequestHandler):
    OnOff = 0
    lvl   = 50

    def do_GET(self):
        logging.info("GET request Path: %s\n", str(self.path) )
        
        if self.path == '/LightBulbESP8266.xml':
            logging.info("Sending light bulb xml\n")

            #self.send_header("Content-type", "text/xml")
            #self.end_headers()
            self.wfile.write(resp)
            self.send_response(200)

        if self.path == '/refresh?':
            logging.info("Sending refresh\n")

            OnOffLvL = str(S.lvl*S.OnOff)
            logging.info("OnOffLvl %f, %d, %s\r\n", S.lvl, S.OnOff, OnOffLvL);
            ref = '{\r\n' \
                  '"status":"HTTP/1.1 200 OK",\r\n' \
                  '"Content-Type":"application/json",\r\n' \
                  '"Cache-Control":"no-cache, private",\r\n' \
                  '"lvl":"'+OnOffLvL+'",\r\n' \
                  '"clr": {\r\n' \
                  '      "r":"50",\r\n' \
                  '      "g":"50",\r\n' \
                  '      "b":"50"\r\n' \
                  '     }\r\n' \
                  '}/r/n'

            #self.send_header("Content-type", "text/json")
            #self.end_headers()
            self.wfile.write(bytes(ref, 'utf-8'))
            self.send_response(200)
            

            
    def do_POST(self):
        strPath = str(self.path)
        logging.info("POST request Path: %s\n", strPath)

        if self.path == '/control?switch=off':
            S.OnOff = 0
            logging.info("Switch on %d\n", S.OnOff)
            
            self.send_header("code", "200")
            self.send_header("Content-type", "text/xml")
            self.send_header("status", "HTTP/1.1 200 OK")
            self.end_headers()
            self.wfile.write(b'{"switch";"off", "code":"200"}') 
            self.send_response(200) 
                     
        if self.path == '/control?switch=on':
            S.OnOff = 1
            logging.info("Switch on %d\n", S.OnOff)
            self.send_header("code", "200")
            self.send_header("Content-type", "text/xml")
            self.send_header("status", "HTTP/1.1 200 OK")
            self.end_headers()
            self.wfile.write(b'{"switch";"on", "code":"200"}')
            self.send_response(200)

        if strPath.startswith('/control?level'):
            S.lvl = int(strPath.split("=",1)[1])
            OnOffLvL = str(S.lvl*S.OnOff)
            ref = '{\r\n' \
                  '"status":"HTTP/1.1 200 OK",\r\n' \
                  '"Content-Type":"application/json",\r\n' \
                  '"Cache-Control":"no-cache, private",\r\n' \
                  '"lvl":"'+OnOffLvL+'",\r\n' \
                  '"clr": {\r\n' \
                  '      "r":"50",\r\n' \
                  '      "g":"50",\r\n' \
                  '      "b":"50"\r\n' \
                  '     }\r\n' \
                  '}/r/n'

            #self.send_header("Content-type", "text/json")
            #self.end_headers()
            self.wfile.write(bytes(ref, 'utf-8'))
            self.send_response(200)
            
def run(server_class=HTTPServer, handler_class=S, port=8190):
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
