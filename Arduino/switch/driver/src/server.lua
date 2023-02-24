local lux = require('luxure')
local cosock = require('cosock')
local json = require('dkjson')
local log = require('log')
local caps = require('st.capabilities')
local utils = require('st.utils')
local commands = require('commands')

local hub_server = {}

function hub_server.start(driver)
  log.info('Starting server')
  local server = lux.Server.new_with(cosock.socket.tcp(), {env='debug'})

  server:listen()
  log.trace('Server listening on ' ..server.ip ..':' ..server.port)

  cosock.spawn(function()
    while true do
      server:tick(log.error)
    end
  end, "server run loop")

  -- Endpoint
  server:post('/push-state', function (req, res)
    log.info('State pushed')
    local body = json.decode(req:get_body())
    --log.info('Body ' .. table.concat(body))
    local device = driver:get_device_info(body.uuid)
    --log.info('Device ' .. table.concat(device))

    --local contactSensor = {}
    --contactSensor.value = body.contact
    --device:emit_event(caps.contactSensor.contact(contactSensor))
    --if body.contact thenopen_closed(device, command)
    --commands.open_closed(device, body.contact)

    res:send('HTTP/1.1 200 OK')
  end)

  driver.server = server
end

return hub_server
