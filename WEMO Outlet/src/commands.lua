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
  
  return commands.send_lan_command(
    device.device_network_id, 'POST', 'ping', ping_data)
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
function commands.on_off(_, device, command)
  local on_off = command.command
  log.info('Starting WEMO on/off ' .. on_off)
  local reqbodyOff = '<?xml version="1.0" encoding="utf-8"?><s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"><s:Body><u:SetBinaryState xmlns:u="urn:Belkin:service:basicevent:1"><BinaryState>0</BinaryState></u:SetBinaryState></s:Body></s:Envelope>'  
  
  local reqbodyOn = '<?xml version="1.0" encoding="utf-8"?><s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"><s:Body><u:SetBinaryState xmlns:u="urn:Belkin:service:basicevent:1"><BinaryState>1</BinaryState></u:SetBinaryState></s:Body></s:Envelope>'

  local reqbody = reqbodyOff
  if on_off == "on" then
    reqbody = reqbodyOn
  end
  
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
                        ["SOAPACTION"]      = "\"urn:Belkin:service:basicevent:1#SetBinaryState\"",                
                        ["content-length"]  = string.len(reqbody)
                },
        sink = ltn12.sink.table(respbody)
    }
log.info('response')
log.info(table.concat(respbody))

 if on_off == 'off' then
      commands.refresh(_, device)
      return device:emit_event(caps.switch.switch.off())
 end
 commands.refresh(_, device)
 return device:emit_event(caps.switch.switch.on())
    
end

------------------------
-- Send LAN HTTP Request
function commands.send_lan_command(url, method, path, body)
  local dest_url = url..'/'..path
  local query = neturl.buildQuery(body or {})
  local res_body = {}
  
  -- HTTP Request
  local ret, code, headers, status = http.request({
    method=method,
    url=dest_url..'?'..query,
    sink=ltn12.sink.table(res_body),
    headers={
      ['Content-Type'] = 'application/x-www-urlencoded'
    }})

  -- Handle response
  if code == 200 then
    return true, res_body
  end
  return false, nil
end

return commands
