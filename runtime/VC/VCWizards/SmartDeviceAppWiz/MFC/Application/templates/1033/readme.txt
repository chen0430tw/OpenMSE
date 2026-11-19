================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : [!output PROJECT_NAME] Project Overview
===============================================================================

The application wizard has created this [!output PROJECT_NAME] application for 
you.  This application not only demonstrates the basics of using the Microsoft 
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your [!output PROJECT_NAME] application.

[!output PROJECT_NAME].vcproj
    This is the main project file for VC++ projects generated using an application wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    application wizard.

[!output APP_HEADER]
    This is the main header file for the application.  It includes other
    project specific headers and declares the [!output APP_CLASS] application class.

[!output APP_IMPL]
    This is the main application source file that contains the application
    class [!output APP_CLASS].

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
    This is the project's main resource file listing of all of the Microsoft 
    Windows resources that the project uses.  It includes the icons, bitmaps, 
    and cursors that are stored in the RES subdirectory.  This file can be directly
    edited in Microsoft Visual C++. Your project resources are in [!output LCID].
    When the .rc file is persisted, the defines in the data section are persisted
    as the hexadecimal version of the numeric value they are defined to rather than
    the friendly name of the define.

res\[!output RC2_FILE_NAME].rc2
    This file contains resources that are not edited by Microsoft 
    Visual C++. You should place all resources not editable by
    the resource editor in this file.
[!endif]

[!if POCKETPC2003_UI_MODEL]
[!output PROJECT_NAME]ppc.rc
    This is the project's main resource file listing of all of the Microsoft Windows
    resources that the project uses when compiling for the Pocket PC platform, or a
    platform that supports the same user interface model.  It includes the icons,
    bitmaps, and cursors that are stored in the RES subdirectory.  This file can be 
    directly edited in Microsoft Visual C++. Your project resources are in 
    [!output LCID]. When the .rc file is persisted, the defines in the data section
    are persisted as the hexadecimal version of the numeric value they are defined to
    rather than the friendly name of the define.

res\[!output RC2_FILE_NAME]ppc.rc2
    This file contains resources that are not edited by Microsoft 
    Visual C++. You should place all resources not editable by
    the resource editor in this file.
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
[!output PROJECT_NAME]sp.rc
    This is the project's main resource file listing of all of the Microsoft Windows
    resources that the project uses when compiling for the Smartphone platform, or a
    platform that supports the same user interface model.  It includes the icons,
    bitmaps, and cursors that are stored in the RES subdirectory.  This file can be 
    directly edited in Microsoft Visual C++. Your project resources are in 
    [!output LCID]. When the .rc file is persisted, the defines in the data section
    are persisted as the hexadecimal version of the numeric value they are defined to
    rather than the friendly name of the define.

res\[!output RC2_FILE_NAME]sp.rc2
    This file contains resources that are not edited by Microsoft 
    Visual C++. You should place all resources not editable by
    the resource editor in this file.
[!endif]
[!if AYGSHELL_UI_MODEL]
[!output PROJECT_NAME]ayg.rc
    This is the project's main resource file listing of all of the Microsoft 
    Windows resources that the project uses when compiling for a platform that
    supports the AYGShell user interface model.  It includes the icons, bitmaps, 
    and cursors that are stored in the RES subdirectory.  This file can be directly
    edited in Microsoft Visual C++. Your project resources are in [!output LCID].
    When the .rc file is persisted, the defines in the data section are persisted
    as the hexadecimal version of the numeric value they are defined to rather than
    the friendly name of the define.

res\[!output RC2_FILE_NAME]ayg.rc2
    This file contains resources that are not edited by Microsoft 
    Visual C++. You should place all resources not editable by
    the resource editor in this file.
[!endif]

res\[!output PROJECT_NAME].ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file.

[!if HAS_SUFFIX]
[!output PROJECT_NAME].reg
    This is an example .reg file that shows you the kind of registration
    settings the framework will set for you.  You can use this as a .reg
[!if APP_TYPE_DLG]
    file to go along with your application.
[!else]
    file to go along with your application or just delete it and rely
    on the default RegisterShellFileTypes registration.
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

For the main frame window:
    The project includes a standard MFC interface.

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    These files contain the frame class [!output MAIN_FRAME_CLASS], which is derived from
    CFrameWnd and controls all SDI frame features.

[!if CMD_BAR_MENUNBUTTON || CMD_BAR_BROWSERSTYLE]
res\Cmdbar.bmp
    This bitmap file is used to create tiled images for the command bar.
    The initial command bar is constructed in the [!output MAIN_FRAME_CLASS] class. 
    Edit this cmdbar bitmap along with the array in [!output MAIN_FRAME_IMPL]
    to add more command bar buttons.
[!endif]

/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
The application wizard creates one document type and one view:

[!output DOC_HEADER], [!output DOC_IMPL] - the document
    These files contain your [!output DOC_CLASS] class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via [!output DOC_CLASS]::Serialize).
[!if HAS_SUFFIX]
    The Document will have the following strings:
        File extension:      [!output FILE_EXTENSION]
        File type ID:        [!output FILE_TYPE_ID]
        Main frame caption:  [!output MAIN_FRAME_CAPTION]
        Doc type name:       [!output DOC_TYPE_NAME]
        Filter name:         [!output FILTER_NAME]
        File new short name: [!output FILE_NEW_NAME_SHORT]
        File type long name: [!output FILE_NEW_NAME_LONG]
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL] - the view of the document
    These files contain your [!output VIEW_CLASS] class.
    [!output VIEW_CLASS] objects are used to view [!output DOC_CLASS] objects.

[!endif]

[!else]
/////////////////////////////////////////////////////////////////////////////

The application wizard creates one dialog class:

[!output DIALOG_HEADER], [!output DIALOG_IMPL] - the dialog
    These files contain your [!output DIALOG_CLASS] class.  This class defines
    the behavior of your application's main dialog.  The dialog's template is
    in the main resource file, which can be edited in Microsoft Visual C++.
[!endif]

[!if HELPSTYLE_WINHELP]
/////////////////////////////////////////////////////////////////////////////

Help Support:

hlp\[!output PROJECT_NAME].hpj
    This file is the help project file used by the help compiler to create
    your application's help file.

hlp\*.bmp
    These are bitmap files required by the standard HELP file topics for
    Microsoft Foundation Class Library standard commands.

hlp\*.rtf
    These files contain the standard help topics for standard MFC
    commands and screen objects.
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

Other Features:
[!if APP_TYPE_DLG]
[!else]
[!if CMD_BAR_MENUONLY]

   Command bar in main frame with menu and adornments
[!else]
[!if CMD_BAR_MENUNBUTTON]
   Menu bar in main frame with menu, buttons, and adornments
[!endif]
[!endif]
[!if STATUS_BAR]
   Initial status bar in main frame
[!endif]
[!endif]
[!if HELPSTYLE_WINHELP]

   Windows CE Help support
[!endif]
[!if DYNAMIC_MFC]

   Uses shared DLL implementation
[!endif]
[!if ACTIVEX_CONTROLS]

ActiveX Controls
    The application includes support to use ActiveX controls.
[!endif]
[!if PRINTING]

Printing and Print Preview support
    The application wizard has generated code to handle the print, print setup, and print preview
    commands by calling member functions in the CView class from the MFC library.
[!endif]
[!if SOCKETS]

Windows Sockets
    The application has support for establishing communications over TCP/IP networks.
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named [!output PROJECT_NAME].pch and a precompiled types file named StdAfx.obj.

[!if STANDARDSHELL_UI_MODEL]
Resource.h
[!endif]
[!if POCKETPC2003_UI_MODEL]
Resourceppc.h
[!endif]
[!if SMARTPHONE2003_UI_MODEL]
Resourcesp.h
[!endif]
[!if AYGSHELL_UI_MODEL]
Resourceayg.h
[!endif]
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.
[!if APP_TYPE_SDI || APP_TYPE_DLG]

If your application uses MFC in a shared DLL, and your application is in a 
language other than the operating system's current language, you will need 
to copy the corresponding localized resources MFC80XXX.DLL to your application
directory ("XXX" stands for the language abbreviation.  For example,
MFC80DEU.DLL contains resources translated to German.)  If you don't do this,
some of the UI elements of your application will remain in the language of the
operating system.
[!endif]

/////////////////////////////////////////////////////////////////////////////
