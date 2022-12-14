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
    -- Monkey patch due to issues
    -- on ltn12 lib to fully sink
    -- JSON payload into table. Last
    -- bracket is missing.
    --
    -- Update below when fixed:  fixed in python device
    log.info('Refresh\n')
    log.info(table.concat(data))
    local raw_data = json.decode(table.concat(data))

    -- Define online status
    device:online()

    -- Refresh Switch Level
    log.trace('Refreshing temperature ')
    local temp = {}
    temp.value = 18
    temp.unit = "F"
    
    device:emit_event(caps.temperatureMeasurement.temperature(temp))
  else
    log.error('failed to poll device state')
    -- Set device as offline
    device:offline()
  end
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
  print('command handler ret      '..ret)
  print('command handler code     '..code)
  --print('command handler headers  '..headers)
  --print('command handler status   '..status)

  -- Handle response
  code = 200
  if code == 200 then
    return true, res_body
  end
  return false, nil
end

return command_handler
