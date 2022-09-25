## MultiWiFiConnect 

For bundling many wifi networks with their passwords together in an array with a priority list and connection based on priority and RSSI in an esp32/8266 (can be ported to any other microcontroller easily) 

Place wifi ssids and passwords in definitions.h at line 31 for higher priority and line 35 for lower priority.

```
//Higher Priority networks
const char* PKNOWN_SSID[] = {"net1"};
const char* PKNOWN_PASSWORD[] = {"net1Password"};


//Lower Priority Networks
const char* KNOWN_SSID[] = {"Net2", "net3", "net4"};
const char* KNOWN_PASSWORD[] = {"net2pass", "net3pass",  "net4pass"};
```

# Usage

The program will scan for all available networks first and then connect to the highest priority network that is available. If the highest priority network is not available, it will connect to the next highest priority network that is available and so on. If none of the networks are available, it will connect to the lower priority networks in a similar manner. 

If no known network is available, it will restart the device and try again.
This can be changed to make the device go into a deep sleep mode instead of restarting or continue with operation without connection.   


make cool stuff.