# arduino_cable_tester

This program checks the continuity of diiferent type of cables.

Arduino Mega 2560 controlled is used in this project.

3 push buttons(Up, Down, Select) are used with one 20x4 Liquid crystal display to select required test.

After test complete LCD will display errors if found.  

Test will automatically start after completion until user stops.

3 LEDs are used to indicate Test result.
- Error found Red LED.
- No Error Green LED.
- If Error found in last test and current test has no Error Amber LED will light up.


## Cable Types
- LK25 M CABLE
- LK25 -> 8 x XLR M - TRS
- 8 x XLR F - TRS -> LK25 M
- LOOM 8 WAY - XLR - TRS
- LOOM 4 WAY - XLR - TRS - EUROBLOCK - TBLOCK
- LOOM 2 WAY - XLR - TRS
- CABLE XLR - TRS
- D-SUB25 - 8 x XLR M - TASCAM
- D-SUB25 - 8 x XLR F - TASCAM
- D-SUB25 - 8 x XLR M - YAMAHA
- D-sub 25 M - 8 x XLR F - YAMAHA
- TRS - 2 x XLR M
- TRS - 2 x XLR F
- ETHERCON - 4 x XLR M
- ETHERCON - 4 x XLR F
- 5 WAY LOOM BNC
- 2 WAY LOOM BNC - RCA
- CABLE BNC - RCA - TS
- ETHERCON 8 Pin CABLE
- MIDI CABLE
- MIDI -> XLR M
- MIDI -> XLR F
- 8 WAY SPEAKER CABLE
- 4 WAY SPEAKER CABLE
- 1 x SPEAKON 2 WAY
- DOM F - XLR M
- XLR F - DOM M
- DO-FILL (CACOM F / 4 SP)
- DO-SUB (1 x SPEAKON 4 WAY)
- 8 WAY XLR LOOM
- 4 WAY XLR LOOM
- 1 XLR CABLE
- LK25 CABLE
- LK25 - XLRM FANOUT
- LK25 - XLRF FAN OUT
- ETHERCON 7 Pin CABLE
- ETHERCON > 4 x XLRM
- 4 x XLRF > ETHERCON
