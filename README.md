# nmusage

nmusage is a network usage measure tool
It's primary use in scripting
Print help
```sh
nmusage -h
```
# Examples / scenarios

human-readable format

```sh
nmusage -hr
```

![Preview](./preview.gif)

fast xdg notification

```sh
 notify-send "Network Data Usage" "$(nwusage -hr)" -t 5000
```
i3/sway

```sh
bindsym $mod+N exec --no-startup-id notify-send "Network Data Usage" "$(nwusage -hr)" -t 5000 -i /usr/share/icons/HighContrast/scalable/devices/network-wireless.svg
```

# Build

```sh
make
```
currently only linux supported/targeted

- **base-devel** required
