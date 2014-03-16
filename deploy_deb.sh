#!/bin/bash

rm -r deploy_deb

mkdir deploy_deb
mkdir deploy_deb/DEBIAN
mkdir deploy_deb/usr
mkdir deploy_deb/usr/bin
mkdir deploy_deb/usr/share
mkdir deploy_deb/usr/share/applications
mkdir deploy_deb/usr/share/icons
mkdir deploy_deb/usr/share/icons/hicolor
mkdir deploy_deb/usr/share/icons/hicolor/128x128
mkdir deploy_deb/usr/share/icons/hicolor/128x128/apps
mkdir deploy_deb/usr/share/pixmaps

cp bin/scibeats deploy_deb/usr/bin/
strip deploy_deb/usr/bin/scibeats
cp scibeats.desktop deploy_deb/usr/share/applications/
cp qrc/icons/scibeats.png deploy_deb/usr/share/icons/hicolor/128x128/apps/scibeats.png
cp qrc/icons/scibeats.png deploy_deb/usr/share/pixmaps/scibeats.png

chmod 0755 deploy_deb/DEBIAN
chmod 0755 deploy_deb/usr
chmod 0755 deploy_deb/usr/bin
chmod 0755 deploy_deb/usr/share
chmod 0755 deploy_deb/usr/share/applications
chmod 0755 deploy_deb/usr/share/icons
chmod 0755 deploy_deb/usr/share/icons/hicolor
chmod 0755 deploy_deb/usr/share/icons/hicolor/128x128
chmod 0755 deploy_deb/usr/share/icons/hicolor/128x128/apps
chmod 0755 deploy_deb/usr/share/pixmaps
chmod 0755 deploy_deb/usr/bin/scibeats
chmod 0644 deploy_deb/usr/share/applications/scibeats.desktop
chmod 0644 deploy_deb/usr/share/icons/hicolor/128x128/apps/scibeats.png
chmod 0644 deploy_deb/usr/share/pixmaps/scibeats.png

VER=`./get_version.sh`
SIZE=`du -cb ./deploy_deb/usr | grep total | awk '{print int($1/1024)}'`
ARCH=`dpkg --print-architecture`
CODENAME=`lsb_release -c | awk '{print $2}'`

echo "Package: scibeats
Version: $VER
Section: net
Priority: optional
Architecture: $ARCH
Installed-Size: $SIZE
Depends: libc6 (>= 2.14), libgcc1 (>= 1:4.1.1), libqt4-webkit (>= 4:4.8.0), libqt4-network (>= 4:4.8.0), libqt4-xml (>= 4:4.8.0), libqtcore4 (>= 4:4.8.0), libqtgui4 (>= 4:4.8.0), portaudio19-dev, libsndfile1-dev, libsamplerate0-dev
Homepage: https://github.com/AlekseyDurachenko/scibeats
Maintainer: Aleksey Durachenko <durachenko.aleksey@gmail.com>
Description: The scientific sound downloader
" > deploy_deb/DEBIAN/control

cd deploy_deb || exit 1
md5deep -r -l usr > ../deploy_deb/DEBIAN/md5sums
cd ..

chmod 644 deploy_deb/DEBIAN/*

fakeroot dpkg-deb --build deploy_deb
mv deploy_deb.deb "scibeats_""$VER~$CODENAME""_""$ARCH"".deb"
lintian "scibeats_""$VER~$CODENAME""_""$ARCH"".deb"
