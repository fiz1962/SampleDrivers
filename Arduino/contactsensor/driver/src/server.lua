local lux = require('luxure')
local cosock = require('cosock').socket
local json = require('dkjson')
local log = require('log')
local caps = require('st.capabilities')
local utils = require('st.utils')

local hub_server = {}

function hub_server.start(driver)
  local server = lux.Server.new_with(cosock.tcp(), {env='debug'})

  -- Register server
  driver:register_channel_handler(server.sock, function ()
    server:tick()
  end)
  
  -- Endpoint
  server:post('/push-state', function (req, res)
    log.info('State pushed')
    local body = json.decode(req:get_body())
    local device = driver:get_device_info(body.uuid)
    local contactSensor = {}
    contactSensor.value = body.contact
    device:emit_event(caps.contactSensor.contact(contactSensor))
    --if body.contact then
    --driver:open_closed(device, body.contact)
    --end
    res:send('HTTP/1.1 200 OK')
  end)
  server:listen()
  driver.server = server
end

return hub_server
