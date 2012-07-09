Name:		rosa-launcher-qtquick
Version:	0.3.2
Release:	1
Epoch:		1
Summary:	ROSA Desktop Application Launcher QtQuick
Group:		Graphical desktop/KDE
License:	GPLv3
URL:		http://www.rosalab.ru/
Source0:	rosa-launcher-qtquick-%{version}.tar.gz

Requires:       kdebase4-workspace
BuildRequires:  kdebase4-workspace-devel
BuildRequires:  kdebase4-devel

%description
ROSA Desktop Application Launcher QtQuick

%files
%defattr(-,root,root)
%_kde_bindir/rosa-launcher-qtquick
%_kde_datadir/rosa-launcher-qtquick/*
%_kde_libdir/kde4/plasma_runner_rosa_services2.so
%_kde_services/plasma-runner-rosa-services2.desktop

#--------------------------------------------------------------------

%prep
%setup -q

%build
%cmake_kde4

%install
%makeinstall_std -C build
