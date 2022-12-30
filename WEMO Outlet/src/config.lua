local config = {}
-- device info
-- NOTE: In the future this information
-- may be submitted through the Developer
-- Workspace to avoid hardcoded values.
config.DEVICE_PROFILE='WEMO-Outlet.v3'
config.DEVICE_TYPE='LAN'
-- SSDP Config
config.MC_ADDRESS='239.255.255.250'
config.MC_PORT=1900
config.MC_TIMEOUT=2
config.MSEARCH=table.concat({
  'M-SEARCH * HTTP/1.1\r\n',
  'ST: urn:Belkin:service:basicevent:1\r\n',
  'MX: 1\r\n',
  'MAN: "ssdp:discover"\r\n',
  'HOST: 239.255.255.250:1900\r\n\r\n'
})
config.SCHEDULE_PERIOD=300
return config
