Name:		rosa-launcher-qtquick
Version:	0.2.2
Release:	18
Epoch:		1
Summary:	ROSA Desktop Application Launcher QtQuick
Group:		Graphical desktop/KDE
License:	GPLv3
URL:		http://www.rosalab.ru/
Source0:	rosa-launcher-qtquick-%{version}.tar.gz

Requires:       kdebase4-workspace
BuildRequires:  kdebase4-workspace-devel

%description
ROSA Desktop Application Launcher QtQuick

%files
%defattr(-,root,root)
%_kde_bindir/rosa-launcher-qtquick
%_kde_datadir/rosa-launcher-qtquick/*
%_kde_libdir/kde4/plasma_runner_rosa_services2.so
%_kde_services/plasma-runner-rosa-services2.desktop
%_datadir/timeframe

#--------------------------------------------------------------------

%prep
%setup -q

%build
%cmake_kde4
cd ../facebook-timeframe-plugin
#cmake . -DCMAKE_INSTALL_PREFIX=/usr
qmake PREFIX=%{buildroot}/%{_prefix}/


%install
%makeinstall_std -C build
#cd ..
ls -l
cd facebook-timeframe-plugin
#mkdir %_prefix/timeframe
%__mkdir -p %{buildroot}/%{_prefix}/share/timeframe
make install
