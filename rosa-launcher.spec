Name:		rosa-launcher
Version:	2.0.0
Release:	15
Epoch:		1
Summary:	ROSA Desktop Application Launcher
Group:		Graphical desktop/KDE
License:	GPLv3
URL:		http://www.rosalab.ru/
Source0:	rosa-launcher-%{version}.tar.gz

Requires:       kdebase4-workspace qjson
BuildRequires:  kdebase4-workspace-devel qjson-devel

%description
ROSA Desktop Application Launcher

%files
%defattr(-,root,root)
%_kde_libdir/kde4/plasma_applet_rosastarter.so
%_kde_libdir/libtimeframe.so
%_kde_datadir/kde4/services/plasma-applet-rosastarter.desktop
%_kde_bindir/rosa-launcher
%_kde_datadir/rosa-launcher/*
%_kde_datadir/locale
%_datadir/timeframe

#--------------------------------------------------------------------

%prep
%setup -q

%build
%cmake_kde4

%install
%makeinstall_std -C build
