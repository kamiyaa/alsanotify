# alsanotify

## Description
Command that changes volume of alsa as well as sends a desktop notification of the new volume

## Dependencies
 - libnotify
 - alsa

## Installation
Install using **make**:
```
$ cd ruiji/src
$ make
# make install
```

## Usage
```
$ alsanotify -c 2	# increases volume by 2%
$ alsanotify -c -2	# decreases volume by 2%
$ alsanotify -s 50	# set volume to 50%
```
