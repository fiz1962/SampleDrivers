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
import config         

class S(BaseHTTPRequestHandler):
 
    
    def do_GET(self):
        logging.info("GET request Path: %s\n", str(self.path) )
        
        if self.path.startswith('/control'):
             global ext_uuid
             global HUB_IP
             global HUB_PORT
        
             if HUB_IP == "":
                self.send_response(400)
                
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
            
        if self.path.startswith('/ping'):
            u = urlparse(self.path)
            qs = parse_qs(u.query)
            ext_uuid = qs['ext_uuid'][0]
            HUB_IP   = qs['ip'][0]
            HUB_PORT = qs['port'][0]      
            
def run(server_class=HTTPServer, handler_class=S, port=8080):
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

    run(port=int(config.devPort.decode()))

