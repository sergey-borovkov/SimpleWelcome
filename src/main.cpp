/*
 * Copyright (c) ROSA Laboratory 2011
 * License: GPLv3
 *
 * Authors:
 * Alexey Yermakov <alexey.yermakov@rosalab.ru>
 *
 * Based on plasma-windowed by:
 * 2006-2008 Aaron Seigo <aseigo@kde.org>
 * 2009 Marco Martin <notmart@gmail.com>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>
#include <KIcon>
#include <KDebug>
#include <kdeclarative.h>

#include "swapp.h"

static const char description[] = I18N_NOOP("ROSA Desktop Application Launcher");
static const char version[] = "2.0";

extern "C"
//KDE_EXPORT int kdemain(int argc, char **argv)
int main(int argc, char *argv[])
{
    KAboutData aboutData("ROSA_Launcher", 0, ki18n("ROSA Desktop Application Launcher"),
                         version, ki18n(description), KAboutData::License_GPL_V3,
                         ki18n("Copyright 2010-2012, ROSALab"));
    aboutData.addAuthor(ki18n("Alexander Skakov"),
                        ki18n("Author"),
                        "alexander.skakov@osinit.ru");
    aboutData.addAuthor(ki18n("Evgeniy Augin"),
                        ki18n("Author"),
                        "evgeniy.augin@osinit.ru");
    aboutData.addAuthor(ki18n("Sergey Borovkov"),
                        ki18n("Author"),
                        "sergey.borovkov@osinit.ru");
    aboutData.addAuthor(ki18n("Julia Mineeva"),
                        ki18n("Author"),
                        "julia.mineeva@osinit.ru");
    aboutData.addAuthor(ki18n("Aleksey Yermakov"),
                        ki18n("Author"),
                        "alexey.yermakov@rosalab.ru");

    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add("s");
    options.add("silent", ki18n("Silent run. Hide application window after start."));
    KCmdLineArgs::addCmdLineOptions(options);

    if (!KUniqueApplication::start()) {
        qDebug() << "SimpleWelcome2 is already running!";
        return 0;
    }

    // Try to setup debugger if special Qt option is passed
    KDeclarative::setupQmlJsDebugger();

    SWApp *app = SWApp::self();
    int rc = app->exec();
    delete app;

    return rc;
}
