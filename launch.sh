#!/bin/bash

FILENAME="src/hello_world_launchd.plist"

PWD=$(pwd)

PLIST="<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<!DOCTYPE plist PUBLIC -//Apple Computer//DTD PLIST 1.0//EN
http://www.apple.com/DTDs/PropertyList-1.0.dtd >
<plist version="1.0">
    <dict>
        <key>Label</key>
        <string>com.wuntee.helloworld</string>
        <key>ProgramArguments</key>
        <array>
            <string>$PWD/bin/hello_world_xpc_service</string>
        </array>
        <key>KeepAlive</key>
        <true/>
        <key>UserName</key>
        <string>wuntee</string>
        <key>StandardOutPath</key>
        <string>$PWD/hello_world_xpc_service.log</string>
        <key>StandardErrorPath</key>
        <string>$PWD/hello_world_xpc_service.log</string>
        <key>MachServices</key>
        <dict>
            <key>com.wuntee.helloworld</key>
            <true/>
        </dict>
    </dict>
</plist>"

echo "[*] Creating plist"
echo $PLIST > $FILENAME
echo "[*] Unloading from launchd"
launchctl unload $FILENAME
echo "[*] Loading to launchd"
launchctl load $FILENAME
echo "[*] Loaded:"
launchctl list | grep wuntee
