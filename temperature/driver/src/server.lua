local lux = require('luxure')
local cosock = require('cosock').socket
local json = require('dkjson')

local hub_server = {}

function hub_server.start(driver)
  local server = lux.Server.new_with(cosock.tcp(), {env='debug'})

  -- Register server
  driver:register_channel_handler(server.sock, function ()
    server:tick()
  end)

  -- Endpoint
  server:post('/push-state', function (req, res)
    local body = json.decode(req:get_body())

    res:send('HTTP/1.1 200 OK')
  end)
  server:listen()
  driver.server = server
end

return hub_server
