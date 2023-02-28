-- require st provided libraries
local capabilities = require "st.capabilities"
local Driver = require "st.driver"
local log = require "log"

-- require custom handlers from driver package
local command_handlers = require "command_handlers"
local discovery = require "discovery"

local lifecycle_handler = {}

function lifecycle_handler.init(driver, device)
  -------------------
  -- Set up scheduled
  -- services once the
  -- driver gets
  -- initialized.

  -- Refresh schedule
  device.thread:call_on_schedule(
    60,
    function ()
      return command_handlers.refresh(nil, device)
    end,
    'Refresh schedule')
end

-----------------------------------------------------------------
-- local functions
-----------------------------------------------------------------
-- this is called once a device is added by the cloud and synchronized down to the hub
local function device_added(driver, device)
  log.info("[" .. device.id .. "] Adding new Hello World device")

  -- set a default or queried state for each capability attribute
  device:emit_event(capabilities.switch.switch.on())
end

-- this is called both when a device is added (but after `added`) and after a hub reboots.
local function device_init(driver, device)
  log.info("[" .. device.id .. "] Initializing Hello World device")

  -- mark device as online so it can be controlled from the app
  device:online()
end

-- this is called when a device is removed by the cloud and synchronized down to the hub
local function device_removed(driver, device)
  log.info("[" .. device.id .. "] Removing Hello World device")
end

-- create the driver object
local hello_world_driver = Driver("helloworld", {
  discovery = discovery.handle_discovery,
  lifecycle_handlers = {
    added = device_added,
    init = lifecycle_handler.init,
    removed = device_removed
  },
  supported_capabilities = {
    capabilities.refresh
  },
  capability_handlers = {
    [capabilities.switch.ID] = {
      [capabilities.switch.commands.on.NAME] = command_handlers.switch_on,
      [capabilities.switch.commands.off.NAME] = command_handlers.switch_off,
    },
    -- Refresh command handler
    [capabilities.refresh.ID] = {
        [capabilities.refresh.commands.refresh.NAME] = command_handlers.refresh
    }
  }
})

-- run the driver
hello_world_driver:run()
