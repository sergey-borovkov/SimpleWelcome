Name:		rosa-launcher
Version:	0.34.9
Release:	1
Epoch:		1
Summary:	ROSA Desktop Application Launcher
Group:		Graphical desktop/KDE
License:	GPLv3
URL:		http://www.rosalab.ru/
Source0:	rosa-launcher-%{version}.tar.gz

Requires:       kdebase4-workspace
BuildRequires:  kdebase4-workspace-devel
BuildRequires:  kdebase4-devel

%description
ROSA Desktop Application Launcher

%files
%defattr(-,root,root)
%_kde_bindir/rosa-launcher
%_kde_libdir/kde4/plasma_applet_rosa-launcher.so
%_kde_datadir/kde4/services/plasma-applet-rosa-launcher.desktop
%_localedir/*

%_kde_libdir/kde4/plasma_applet_rosastarter.so
%_kde_datadir/kde4/services/plasma-applet-rosastarter.desktop

%_kde_libdir/kde4/plasma_runner_rosa_services.so
%_kde_datadir/kde4/services/plasma-runner-rosa-services.desktop

%_kde_datadir/icons/rosa/apps/128/rosalauncher.png
%_kde_datadir/apps/rosa-launcher/*

#--------------------------------------------------------------------

%prep
%setup -q

%build
%make build

%install
%makeinstall_std

%ifarch x86_64
mv %{buildroot}/usr/lib %{buildroot}/usr/lib64
%endif
