PebSlee
==============

# Description

PebSlee is smart sleep alarm and monitoring for sleep patterns application working on Pebble watches.

Features include:

* Smart alarm - the alarm is set in interval, and the application chooses a moment in this interval, when your sleep is most light - this avoids the dizziness and the feeling of missing enough sleep.

* Autonomous application - no need of applications running on your smart phone - you can configure and managed the alarms completely on your smart watch.

* Reduced power usage - the application comes even with its first version with optimized algorithm for saving the energy of your Pebble.

* Background process - the application is able to run in foreground and to suppress the background processes like fitness apps, that can interrupt the app execution during night.

* Clean and clear interface

for more details visit http://www.pebslee.com/

# Version information

## v.1.6

* Translations in French - ready for German and Spanish
* Option for snooze
* Remote command activated for toggling tracking from the phone
* Stability - memory leaks removed.

## v.1.5

* Back button on main windows works only when sleep is not active (stopped) - can be used to light up display while active
* Sensitivity of falling asleep (slow, normal, and fast)
* Sensitivity of movement (not sensitive, normal, very sensitive)

## v.1.4

* Fix stats persistence size (10)
* Action menu with - View stats, Clear Stats, Set time, Version
* DB and version maintanance

## v.1.3

Fix bug in storing statistics when having more than 10 days tracking. Note that this version will delete old statistics data.

## v.1.2

* Sync Screen
* Persist motion values in byte scale
* Sync data - set start end time from phone
* Sync data - send last sleep motion values to phone app

[sync]https://github.com/npenkov/pebslee-pebble/blob/master/screenshots/sync_screen.png

## v.1.1

* Store last 10 sleep statistics
* Stat screen supports navigation between sleep stats
* New UI for stat screen (avoid Awake time, show date and start/end of sleep)

[stats]https://github.com/npenkov/pebslee-pebble/blob/master/screenshots/v11_stats_screen.png

## v.1.0

Intial version:

* Configuration screen - configure your alarm period
* Main screen with shortcut modes
* Background worker, during sleep monitoring
* Basic statistic screen for last screen

[tracking]https://github.com/npenkov/pebslee-pebble/blob/master/screenshots/withalarm-tracking.png
[stats]https://github.com/npenkov/pebslee-pebble/blob/master/screenshots/stats.png
[configure]https://github.com/npenkov/pebslee-pebble/blob/master/screenshots/config-alarm-set-time.png