local caps = require('st.capabilities')
local utils = require('st.utils')
local neturl = require('net.url')
local log = require('log')
local json = require('dkjson')
local cosock = require "cosock"
local http = cosock.asyncify "socket.http"
local ltn12 = require('ltn12')

local commands = {}

---------------
-- Ping command
function commands.ping(address, port, device)
  local ping_data = {ip=address, port=port, ext_uuid=device.id}
  log.info('Sending ping')
  log.info(table.concat(ping_data))
  
  return 1
  --return commands.send_lan_command(
  --  device.device_network_id, 'POST', 'ping', ping_data)
end
------------------
-- Refresh command
function commands.refresh(_, device)
  log.info('Starting WEMO refresh')
    local reqbody = '<?xml version="1.0" encoding="utf-8"?><s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"><s:Body><u:GetBinaryState xmlns:u="urn:Belkin:service:basicevent:1"><BinaryState>1</BinaryState></u:GetBinaryState></s:Body></s:Envelope>'

    local respbody = {} 
    local  body, code, headers, status = http.request {
        method = "POST",
        url = device.device_network_id .. '/upnp/control/basicevent1',
        source = ltn12.source.string(reqbody),
        headers = 
                {
                        ["Accept"] = "*/*",
                        ["Accept-Encoding"] = "gzip, deflate",
                        ["Accept-Language"] = "en-us",
                        ["Content-Type"]    = "text/xml; charset=\"utf-8\"",
                        ["SOAPACTION"]      = "\"urn:Belkin:service:basicevent:1#GetBinaryState\"",                
                        ["content-length"]  = string.len(reqbody)
                },
        sink = ltn12.sink.table(respbody)
    }

  -- Check success
  if code == 200 then
    log.info('Refresh ok\n')
    local r =table.concat(respbody)
    log.info(r)
    local switch = {}
    if string.find(r, "<BinaryState>0</BinaryState>") then
        switch.value = "off"
    else
        switch.value = "on"
    end
    device:emit_event(caps.switch.switch(switch))
    device:online()
  else
    log.error('failed to poll device state')
    -- Set device as offline
    device:offline()
  end
end

-- Switch command
function commands.switch_on(driver, device, command)
  log.debug(string.format("[%s] calling set_power(on)", device.device_network_id))
    local reqbodyOn = '<?xml version="1.0" encoding="utf-8"?><s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"><s:Body><u:SetBinaryState xmlns:u="urn:Belkin:service:basicevent:1"><BinaryState>1</BinaryState></u:SetBinaryState></s:Body></s:Envelope>'
    local respbody = {} 
    local  body, code, headers, status = http.request {
        method = "POST",
        url = device.device_network_id .. '/upnp/control/basicevent1',
        source = ltn12.source.string(reqbodyOn),
        headers = 
                {
                        ["Accept"] = "*/*",
                        ["Accept-Encoding"] = "gzip, deflate",
                        ["Accept-Language"] = "en-us",
                        ["Content-Type"]    = "text/xml; charset=\"utf-8\"",
                        ["SOAPACTION"]      = "\"urn:Belkin:service:basicevent:1#SetBinaryState\"",                
                        ["content-length"]  = string.len(reqbodyOn)
                },
        sink = ltn12.sink.table(respbody)
    }
log.info('response')
log.info(table.concat(respbody))
  device:emit_event(caps.switch.switch.on())
end

-- callback to handle an `off` capability command
function commands.switch_off(driver, device, command)
  log.debug(string.format("[%s] calling set_power(off)", device.device_network_id))
  local reqbodyOff = '<?xml version="1.0" encoding="utf-8"?><s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"><s:Body><u:SetBinaryState xmlns:u="urn:Belkin:service:basicevent:1"><BinaryState>0</BinaryState></u:SetBinaryState></s:Body></s:Envelope>'  

    local respbody = {} 
    local  body, code, headers, status = http.request {
        method = "POST",
        url = device.device_network_id .. '/upnp/control/basicevent1',
        source = ltn12.source.string(reqbodyOff),
        headers = 
                {
                        ["Accept"] = "*/*",
                        ["Accept-Encoding"] = "gzip, deflate",
                        ["Accept-Language"] = "en-us",
                        ["Content-Type"]    = "text/xml; charset=\"utf-8\"",
                        ["SOAPACTION"]      = "\"urn:Belkin:service:basicevent:1#SetBinaryState\"",                
                        ["content-length"]  = string.len(reqbodyOff)
                },
        sink = ltn12.sink.table(respbody)
    }
log.info('response')
log.info(table.concat(respbody))
  device:emit_event(caps.switch.switch.off())
end

return commands
