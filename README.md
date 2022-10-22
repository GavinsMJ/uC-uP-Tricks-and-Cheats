## uC-uP-Tricks-and-Cheats
A few tricks for doing things faster and better with resource limited controllers 


# MultiWiFiConnect 
For bundling many wifi networks with their passwords together in an array with a priority list and connection based on priority and RSSI in an esp32/8266 (can be ported to any other microcontroller easily).

# C_Strings
Avoid using the String class completely - uses far less ram, no mysterious bugs due to memory fragmentation also minimise use of Serial class, only use it in one place to output one character. Makes it easy to port code to different displays.


# Separate_delimited_Strings
Separate delimited strings into an array of strings. Useful for parsing CSV files or other delimited strings.