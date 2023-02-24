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

currentState = 'off'

class S(BaseHTTPRequestHandler):

    def do_GET(self):
        global currentState
        logging.info("GET request Path: %s\n", str(self.path) )
        
        if self.path == '/SwitchESP8266.xml':
            logging.info("Sending contact sensor xml\n")

            self.wfile.write(config.resp)
            self.send_response(200)
 
        if self.path == '/refresh?':
            logging.info("Sending refresh\n")

            ref = '{\r\n' \
                  '"status":"HTTP/1.1 200 OK",\r\n' \
                  '"Content-Type":"application/json",\r\n' \
                  '"Cache-Control":"no-cache, private",\r\n' \
                  '"switch":"' + currentState +'"\r\n' \
                  '}/r/n'

            self.wfile.write(bytes(ref, 'utf-8'))
            self.send_response(200)
            
    def do_POST(self):
        global ext_uuid
        global HUB_IP
        global HUB_PORT
        global currentState

        strPath = str(self.path)
        logging.info("POST request Path: %s\n", strPath)
            
        if self.path.startswith('/ping'):
            u = urlparse(self.path)
            qs = parse_qs(u.query)
            ext_uuid = qs['ext_uuid'][0]
            HUB_IP   = qs['ip'][0]
            HUB_PORT = qs['port'][0]

        if self.path.startswith('/control'):
            u = urlparse(self.path)
            qs = parse_qs(u.query)
            currentState = qs['value'][0]
            logging.info("Control " + currentState + "\n")
            
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

