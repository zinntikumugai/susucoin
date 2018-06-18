
Debian
====================
This directory contains files used to package susucoind/susucoin-qt
for Debian-based Linux systems. If you compile susucoind/susucoin-qt yourself, there are some useful files here.

## susucoin: URI support ##


susucoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install susucoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your susucoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/susucoin128.png` to `/usr/share/pixmaps`

susucoin-qt.protocol (KDE)

