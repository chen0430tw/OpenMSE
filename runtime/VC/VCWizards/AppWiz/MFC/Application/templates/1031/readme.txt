================================================================================
    MICROSOFT FOUNDATION CLASS-BIBLIOTHEK: [!output PROJECT_NAME]-Projektübersicht
===============================================================================

Der Anwendungs-Assistent hat diese [!output PROJECT_NAME]-Anwendung für Sie 
erstellt. Diese Anwendung zeigt nicht nur die Grundlagen der Verwendung von
Microsoft Foundation Classes, sondern dient auch als Ausgangspunkt für das
Schreiben Ihrer Anwendung.

Diese Datei enthält eine Zusammenfassung dessen, was Sie in jeder der Dateien
finden, aus denen Ihre [!output PROJECT_NAME]–Anwendung besteht.

[!output PROJECT_NAME].vcproj
    Dies ist die Hauptprojektdatei für VC++-Projekte, die mithilfe eines 
    Anwendungs-Assistenten erstellt werden. 
    Sie enthält Informationen über die Version von Visual C++, in der die Datei 
    erzeugt wurde, sowie Informationen über die Plattformen, Konfigurationen und 
    Projektfunktionen, die mit dem Anwendungs-Assistenten ausgewählt wurden.

[!output APP_HEADER]
    Dies ist die Hauptheaderdatei für die Anwendung. Sie enthält weitere
    projektspezifische Header (einschließlich "Resource.h") und deklariert
    die [!output APP_CLASS]-Anwendungsklasse.

[!output APP_IMPL]
    Dies ist die Hauptquelldatei der Anwendung, die die [!output APP_CLASS]-
    Anwendungsklasse enthält.

[!output PROJECT_NAME].rc
    Dies ist eine Auflistung aller Microsoft Windows-Ressourcen, die das
    Programm verwendet. Sie enthält die Symbole, Bitmaps und Cursor, die im 
    Unterverzeichnis "RES" gespeichert werden. Diese Datei kann direkt in 
    Microsoft Visual C++ bearbeitet werden. Ihre Projektressourcen befinden sich 
    in [!output LCID].

res\[!output PROJECT_NAME].ico
    Dies ist eine Symboldatei, die als Symbol der Anwendung verwendet wird. 
    Dieses Symbol ist in der Hauptressourcendatei "[!output PROJECT_NAME].rc" 
    enthalten.

res\[!output RC2_FILE_NAME].rc2
    Diese Datei enthält Ressourcen, die nicht von Microsoft Visual C++
    bearbeitet werden. Sie sollten alle Ressourcen, die nicht mit dem
    Ressourcen-Editor bearbeitet werden können, in dieser Datei platzieren.
[!if CONTAINER_SERVER || FULL_SERVER || MINI_SERVER || AUTOMATION || HAS_SUFFIX]
[!if !HTML_EDITVIEW]

[!output PROJECT_NAME].reg
    Dies ist eine REG-Beispieldatei, die Ihnen die Art der Registrierungs-
    einstellungen zeigt, die das Framework für Sie festlegt. Sie können diese 
    als REG-Datei verwenden.
[!if APP_TYPE_DLG]
    Datei für Ihre Anwendung.
[!else]
    Datei für Ihre Anwendung. Oder löschen Sie sie einfach, und vertrauen
    Sie auf die standardmäßige RegisterShellFileTypes–Registrierung.
[!endif]

[!if AUTOMATION]
[!output SAFE_IDL_NAME].idl
    Diese Datei enthält den Interface Description Language-Quellcode für die
    Typbibliothek Ihrer Anwendung.
[!endif]
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

Für das Hauptrahmenfenster:
[!if PROJECT_STYLE_EXPLORER]
    Windows Explorer-Format: Das Projekt enthält eine Oberfläche, die der von
    Windows Explorer ähnelt, mit zwei Rahmen.
[!else]
    Das Projekt enthält eine standardmäßige MFC-Schnittstelle.
[!endif]

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    Diese Datei enthält die [!output MAIN_FRAME_CLASS]–Rahmenklasse, die von
[!if APP_TYPE_MDI]
    CMDIFrameWnd abgeleitet wird und alle MDI-Rahmenfunktionen steuert.
[!else]
    CFrameWnd abgeleitet wird und alle SDI-Rahmenfunktionen steuert.
[!endif]
[!if PROJECT_STYLE_EXPLORER]

[!output TREE_VIEW_HEADER], [!output TREE_VIEW_IMPL]
    Diese Datei enthält die linke [!output TREE_VIEW_CLASS]–Rahmenklasse, die
    von CtreeView abgeleitet wird.
[!endif]

[!if DOCKING_TOOLBAR]
res\Toolbar.bmp
    Diese Bitmapdatei wird verwendet, um gekachelte Bilder für die Symbolleiste 
    zu erstellen.
    Die ursprüngliche Symbolleiste und Statusleiste werden in der [!output MAIN_FRAME_CLASS]-
    Klasse erstellt. Bearbeiten Sie diese Symbolleistenbitmap mithilfe des
    Ressourcen-Editors, und aktualisieren Sie das IDR_MAINFRAME TOOLBAR-Array
    in "[!output PROJECT_NAME].rc", um Symbolleisten-Schaltflächen hinzuzufügen.
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

res\IToolbar.bmp
    Die Bitmapdatei wird verwendet, um gekachelte Bilder für die Symbolleiste
    zu erstellen, wenn Ihre Serveranwendung innerhalb eines anderen Containers
    direkt aktiviert wird. Diese Symbolleiste wird in der [!output INPLACE_FRAME_CLASS]–
    Klasse erstellt. Diese Bitmap ähnelt der Bitmap in "res\Toolbar.bmp",
    außer dass viele Nichtserverbefehle entfernt wurden.

[!endif]
[!endif]
[!if APP_TYPE_MDI]
/////////////////////////////////////////////////////////////////////////////

Für das untergeordnete Rahmenfenster:

"ChildFrm.h", "ChildFrm.cpp"
    Diese Dateien definieren und implementieren die [!output CHILD_FRAME_CLASS]–
    Klasse, die die untergeordneten Fenster in einer MDI-Anwendung unterstützt.

[!endif]
/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
[!if !DB_VIEW_NO_FILE]
Der Anwendungs-Assistent erstellt einen Dokumenttyp und eine Ansicht:

[!output DOC_HEADER], [!output DOC_IMPL] – das Dokument
    Diese Dateien enthalten Ihre [!output DOC_CLASS]–Klasse. Bearbeiten Sie 
    diese Dateien, um spezielle Dokumentdaten hinzuzufügen und Dateispeicherung
    und –ladung (über [!output DOC_CLASS]::Serialize) zu implementieren.
[!if HAS_SUFFIX && !HTML_EDITVIEW]
    Das Dokument weist folgende Zeichenfolgen auf:
        Dateierweiterung:          [!output FILE_EXTENSION]
        Dateityp-ID:               [!output FILE_TYPE_ID]
        Hauptrahmenüberschrift:    [!output MAIN_FRAME_CAPTION]
        Dokumenttypname:           [!output DOC_TYPE_NAME]
        Filtername:                [!output FILTER_NAME]
        Neuer Dateikurzname:       [!output FILE_NEW_NAME_SHORT]
        Langer Name des Dateityps: [!output FILE_NEW_NAME_LONG]
[!endif]
[!else]
Der Anwendungs-Assistent erstellt eine Ansicht:
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL] – die Ansicht des Dokuments
    Diese Dateien enthalten Ihre [!output VIEW_CLASS]–Klasse.
[!if !DB_VIEW_NO_FILE]
    [!output VIEW_CLASS]-Objekte werden verwendet, um [!output DOC_CLASS]-
    Objekte anzuzeigen.
[!endif]

[!if APP_TYPE_MDI]
res\[!output PROJECT_NAME]Doc.ico
    Dies ist eine Symboldatei, die als Symbol für untergeordnete MDI-Fenster
    für die [!output DOC_CLASS]-Klasse verwendet wird. Dieses Symbol ist in der 
    Hauptressourcendatei "[!output PROJECT_NAME].rc" enthalten.
[!endif]

[!endif]

[!if DB_VIEW_NO_FILE || DB_VIEW_WITH_FILE]
/////////////////////////////////////////////////////////////////////////////

Datenbankunterstützung:

[!output ROWSET_HEADER], [!output ROWSET_IMPL]
    Diese Dateien enthalten Ihre [!output ROWSET_CLASS]–Klasse. Diese Klasse 
    wird verwendet, um auf die Datenquelle zuzugreifen, die Sie im Assistenten 
    ausgewählt haben.
[!if DB_VIEW_NO_FILE]
    Es wird keine Serialisierungsunterstützung hinzugefügt.
[!else]
    Serialisierungsunterstützung wurde hinzugefügt.
[!endif]
[!endif]
[!if CONTAINER || FULL_SERVER || MINI_SERVER || CONTAINER_SERVER]
/////////////////////////////////////////////////////////////////////////////

Der Anwendungs-Assistent hat außerdem OLE-spezifische Klassen erstellt.

[!if CONTAINER || CONTAINER_SERVER]
[!output CONTAINER_ITEM_HEADER], [!output CONTAINER_ITEM_IMPL]
    Diese Dateien enthalten Ihre [!output CONTAINER_ITEM_CLASS]–Klasse. Diese 
    Klasse wird verwendet, um OLE-Objekte zu bearbeiten. OLE-Objekte werden 
    normalerweise von der [!output VIEW_CLASS]–Klasse angezeigt und als Teil der 
    [!output DOC_CLASS]–Klasse serialisiert.
[!if ACTIVE_DOC_CONTAINER]
    Das Programm unterstützt aktive Dokumente innerhalb seines Rahmens.
[!endif]
[!endif]
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

[!output SERVER_ITEM_HEADER], [!output SERVER_ITEM_IMPL]
    Diese Dateien enthalten [!output SERVER_ITEM_CLASS]. Diese Klasse wird 
    verwendet, um Ihre [!output DOC_CLASS]–Klasse mit dem OLE-System zu 
    verbinden und optional Verknüpfungen zu Ihrem Dokument bereitzustellen.
[!if ACTIVE_DOC_SERVER]
    Das Projekt unterstützt das Erstellen und Verwalten aktiver Dokumente.
[!endif]

[!output INPLACE_FRAME_HEADER], [!output INPLACE_FRAME_IMPL]
    Diese Dateien enthalten [!output INPLACE_FRAME_CLASS]. Diese Klasse wird von
    COleIPFrameWnd abgeleitet und steuert alle Rahmenfunktionen während der 
    direkten Aktivierung.
[!endif]

[!if SUPPORT_COMPOUND_FILES]
    Das Projekt unterstützt Verbunddateien. Das Verbunddateiformat speichert ein 
    Dokument, das mindestens ein Automatisierungsobjekt enthält, in einer Datei 
    und ermöglicht weiterhin den Zugriff auf die einzelnen Objekte.
[!endif]
[!endif]
[!else]

/////////////////////////////////////////////////////////////////////////////

[!if AUTOMATION]
Der Anwendungs-Assistent erstellt eine Dialogfeldklasse und eine Automatisierungsproxyklasse:
[!else]
Der Anwendungs-Assistent erstellt eine Dialogfeldklasse:
[!endif]

[!output DIALOG_HEADER], [!output DIALOG_IMPL] – das Dialogfeld
    Diese Dateien enthalten Ihre [!output DIALOG_CLASS]–Klasse. Diese Klasse 
    bestimmt das Verhalten des Hauptdialogfelds Ihrer Anwendung. Die Vorlage des 
    Dialogfelds befindet sich in der Datei "[!output PROJECT_NAME].rc", die in 
    Microsoft Visual C++ bearbeitet werden kann.
[!if AUTOMATION]

[!output DIALOG_AUTO_PROXY_HEADER], [!output DIALOG_AUTO_PROXY_IMPL] – das Automatisierungsobjekt
    Diese Dateien enthalten Ihre [!output DIALOG_AUTO_PROXY_CLASS]–Klasse. Diese 
    Klasse wird als Automatisierungsproxyklasse für Ihr Dialogfeld bezeichnet,
    da sie für das Verfügbarmachen von Automatisierungsmethoden und 
    –eigenschaften zuständig ist, mit deren Hilfe Automatisierungscontroller 
    auf Ihr Dialogfeld zugreifen können. Diese Methoden und Eigenschaften werden 
    nicht von der Dialogfeldklasse direkt verfügbar gemacht, da es bei modalen, 
    dialogfeldbasierten MFC-Anwendungen übersichtlicher und einfacher ist, das 
    Automatisierungsobjekt von der Benutzeroberfläche zu trennen.
[!endif]
[!endif]

[!if CONTEXT_HELP]
/////////////////////////////////////////////////////////////////////////////

Hilfeunterstützung:

[!if HELPSTYLE_HTML]
hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhp
    Diese Datei ist eine Hilfeprojektdatei. Sie enthält die Daten, die 
    zum Kompilieren der Hilfedateien in einer CHM-Datei erforderlich sind.

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhc
    Diese Datei listet den Inhalt des Hilfeprojekts auf.

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhk
    Diese Datei enthält einen Index der Hilfethemen.

hlp\afxcore.htm
    Diese Datei enthält die Standardhilfethemen für standardmäßige
    MFC-Befehle und Bildschirmobjekte. Fügen Sie eigene Hilfethemen dieser Datei hinzu.

[!if PRINTING]
hlp\afxprint.htm
    Diese Datei enthält die Hilfethemen zu den Druckbefehlen.

[!endif]
makehtmlhelp.bat
    Diese Datei wird vom Buildsystem zum Kompilieren der Hilfedateien verwendet.

hlp\Images\*.gif
    Dies sind Bitmapdateien, die für standardmäßige Hilfedateithemen zu den
    Microsoft Foundation Class Library-Standardbefehlen erforderlich sind.

[!else]
hlp\[!output PROJECT_NAME].hpj
    Diese Datei ist die Hilfeprojektdatei, die vom Hilfecompiler verwendet
    wird, um die Hilfedatei Ihrer Anwendung zu erstellen.

hlp\*.bmp
    Dies sind Bitmapdateien, die für standardmäßige Hilfedateithemen zu den
    Microsoft Foundation Class Library-Standardbefehlen erforderlich sind.

hlp\*.rtf
    Diese Dateien enthalten die Standardhilfethemen für standardmäßige
    MFC-Befehle und -Bildschirmobjekte.
[!endif]
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SPLITTER || MAPI || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

Weitere Funktionen:
[!if ACTIVEX_CONTROLS]

ActiveX-Steuerelemente
    Die Anwendung unterstützt die Verwendung von ActiveX-Steuerelementen.
[!endif]
[!if PRINTING]

Druck- und Druckvorschauunterstützung
    Der Anwendungs-Assistent hat Code generiert, um die Befehle für Drucken, 
    Druckeinrichtung und Druckvorschau zu behandeln, indem Memberfunktionen in 
    der CView-Klasse aus der MFC-Bibliothek aufgerufen werden.
[!endif]
[!if DB_SUPPORT_HEADER_ONLY && !APP_TYPE_DLG]

Datenbankunterstützung
    Der Anwendungs-Assistent hat die grundlegende Datenbankunterstützung für Ihr 
    Programm hinzugefügt. 
    Nur die erforderlichen Dateien wurden eingeschlossen.
[!endif]
[!if SPLITTER && !APP_TYPE_DLG]

Splitterfenster
    Der Anwendungs-Assistent hat Unterstützung für Splitterfenster für Ihre 
    Anwendungsdokumente hinzugefügt.
[!endif]
[!if MAPI]

MAPI-Unterstützung
    Das generierte Projekt enthält den erforderlichen Code für das Erstellen, 
    Bearbeiten, Übermitteln und Speichern von E-Mail-Nachrichten.
[!endif]
[!if SOCKETS]

Windows Sockets
    Die Anwendung unterstützt die Kommunikation über TCP/IP-Netzwerke.
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

Weitere Standarddateien:

"StdAfx.h", "StdAfx.cpp"
    Diese Dateien werden verwendet, um eine vorkompilierte Headerdatei
    (PCH-Datei) mit dem Namen "[!output PROJECT_NAME].pch.pch2 und eine 
    vorkompilierte Typendatei mit dem Namen "StdAfx.obj" zu erstellen.

"Resource.h"
    Dies ist die Standardheaderdatei, die neue Ressourcen-IDs definiert.
    Microsoft Visual C++ liest und aktualisiert diese Datei.

[!if MANIFEST]
[!output PROJECT_NAME].manifest
	Anwendungsmanifestdateien werden von Windows XP verwendet, um eine 
	Anwendungsabhängigkeit von verschiedenen Versionen paralleler Assemblys zu 
	beschreiben.
	Das Ladeprogramm verwendet diese Informationen, um die entsprechende 
	Assembly aus dem Assemblycache oder privat aus der Anwendung zu laden. Das
	Anwendungsmanifest kann zur Verteilung als externe Manifestdatei
	enthalten sein, die im gleichen Ordner installiert ist wie die ausführbare 
	Datei der Anwendung, oder sie kann in Form einer Ressource in der 
	ausführbaren Datei enthalten sein. 
[!endif]
/////////////////////////////////////////////////////////////////////////////

Weitere Hinweise:

Der Anwendungs-Assistent verwendet "TODO:", um auf Teile des Quellcodes
hinzuweisen, die Sie ergänzen oder anpassen sollten.
[!if APP_TYPE_MDI || APP_TYPE_SDI || APP_TYPE_DLG || APP_TYPE_MTLD]

Wenn Ihre Anwendung MFC in einer freigegebenen DLL verwendet, müssen Sie die 
MFC-DLLs verteilen. Wenn die Anwendung eine andere Sprache als die des Gebietsschemas 
des Betriebssystems verwendet, müssen Sie außerdem die entsprechenden lokalisierten Ressourcen "MFC80XXX.DLL" verteilen. Weitere Informationen zu diesen beiden Themen 
finden Sie im Abschnitt zum Verteilen von Visual C++-Anwendungen in der MSDN-
Dokumentation.
[!endif]

/////////////////////////////////////////////////////////////////////////////