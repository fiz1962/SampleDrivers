local caps = require('st.capabilities')
local utils = require('st.utils')
local neturl = require('net.url')
local log = require('log')
local json = require('dkjson')
local cosock = require "cosock"
local http = cosock.asyncify "socket.http"
local ltn12 = require('ltn12')

local command_handler = {}

---------------
-- Ping command
function command_handler.ping(address, port, device)
  local ping_data = {ip=address, port=port, ext_uuid=device.id}
  log.info('Sending ping')
  log.info(table.concat(ping_data))
  
  return command_handler.send_lan_command(
    device.device_network_id, 'POST', 'ping', ping_data)
end
------------------
-- Refresh command
function command_handler.refresh(_, device)
  local success, data = command_handler.send_lan_command(
    device.device_network_id,
    'GET',
    'refresh')

  -- Check success
  if success then
     log.info('Refresh\n')
    log.info(table.concat(data))
    local raw_data = json.decode(table.concat(data))

    -- Define online status
    device:online()

    -- Refresh Switch Level
    log.trace('Refreshing contact sensor ')
    local contactSensor = {}
    contactSensor.value = raw_data.contact
    log.info('ContactSensor dump')
    log.info(raw_data.contact)
    device:emit_event(caps.contactSensor.contact(contactSensor))
  else
    log.error('failed to poll device state')
    -- Set device as offline
    device:offline()
  end
end

-- Switch commad
function command_handler.open_closed(device, command)
  log.info('Command ' .. command)
    local contactSensor = {}
    contactSensor.value = command
    device:emit_event(caps.contactSensor.contact(contactSensor))
end
------------------------
-- Send LAN HTTP Request
function command_handler.send_lan_command(url, method, path, body)
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

return command_handler
