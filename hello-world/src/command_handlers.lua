local log = require "log"
local capabilities = require "st.capabilities"

local command_handlers = {}

-- Refresh command
function command_handlers.refresh(_, device)

     log.info('Refresh\n')
    
    -- Define online status
    device:online()

    -- Refresh temperature
    log.trace('Refreshing temperature ')
    local temp = {}
    temp.value = math.random(72, 76)
    temp.unit = "F"
    
    device:emit_event(capabilities.temperatureMeasurement.temperature(temp))

end

-- callback to handle an `on` capability command
function command_handlers.switch_on(driver, device, command)
  log.debug(string.format("[%s] calling set_power(on)", device.device_network_id))
  device:emit_event(capabilities.switch.switch.on())
end

-- callback to handle an `off` capability command
function command_handlers.switch_off(driver, device, command)
  log.debug(string.format("[%s] calling set_power(off)", device.device_network_id))
  device:emit_event(capabilities.switch.switch.off())
end

return command_handlers
