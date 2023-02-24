local Driver = require('st.driver')
local capabilities = require('st.capabilities')

-- local imports
local discovery = require('discovery')
local lifecycles = require('lifecycles')
local commands = require('commands')
local server = require('server')

--------------------
-- Driver definition
local driver =
  Driver(
    'Edge Switch',
    {
      discovery = discovery.start,
      lifecycle_handlers = lifecycles,
      supported_capabilities = {
        capabilities.refresh
      },
      capability_handlers = {
	[capabilities.switch.ID] = {
	  [capabilities.switch.commands.on.NAME] = commands.switch_on,
	  [capabilities.switch.commands.off.NAME] = commands.switch_off,
    	},
        -- Refresh command handler
        [capabilities.refresh.ID] = {
          [capabilities.refresh.commands.refresh.NAME] = commands.refresh
        }
      }
    }
  )

-----------------------------
-- Initialize Hub server
-- that will open port to
-- allow bidirectional comms.
server.start(driver)

--------------------
-- Initialize Driver
driver:run()
