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
    def _set_response(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_GET(self):
        logging.info("GET request,\nPath: %s\nHeaders:\n%s\nEnd headers\n", str(self.path), str(self.headers))
        self.send_response(200)
        
        if self.path == '/LightBulbESP8266.xml':
            logging.info("Sending light bulb xml\n")

            self.send_header("Content-type", "text/xml")
            self.end_headers()
            self.wfile.write(resp)
            self.send_response(200)

        if self.path == '/refresh?':
            logging.info("Sending refresh\n")

            OnOffLvL = str(lvl*OnOff)
            ref = '{' \
                  '"lvl":"'+OnOffLvL+'",\r\n' \
                  '"clr": {\r\n' \
                  '      "r":"50",\r\n' \
                  '      "g":"50",\r\n' \
                  '      "b":"50"\r\n' \
                  '     }\r\n'
            # final curly bracket added in original light bulb driver code

            self.send_header("Content-type", "text/json")
            self.end_headers()
            self.wfile.write(ref)
            self.send_response(200)
            
        if self.path == '/control?switch=off':
            logging.info("Switch off\n")
            OnOff = 0
            
            self.send_header("code", "200")
            self.send_header("Content-type", "text/xml")
            self.send_header("status", "HTTP/1.1 200 OK")
            self.end_headers()
            self.wfile.write(b'{"switch";"off", "code":"200"}') 
            self.send_response(200) 
                     
        if self.path == '/control?switch=on':
            logging.info("Switch on\n")
            OnOff = 1
            self.send_header("code", "200")
            self.send_header("Content-type", "text/xml")
            self.send_header("status", "HTTP/1.1 200 OK")
            self.end_headers()
            self.wfile.write(b'{"switch";"on", "code":"200"}')
            self.send_response(200)
            
    def do_POST(self):
        logging.info("GET request,\nPath: %s\nHeaders:\n%s\nEnd headers\n", str(self.path), str(self.headers))

        #content_length = int(self.headers['Content-Length']) # <--- Gets the size of data
        #post_data = self.rfile.read(content_length) # <--- Gets the data itself
        #l#ogging.info("POST request,\nPath: %s\nHeaders:\n%s\n\nBody:\n%s\n",
        #        str(self.path), str(self.headers), post_data.decode('utf-8'))

        #self.wfile.write("POST request for {}".format(self.path).encode('utf-8'))


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
