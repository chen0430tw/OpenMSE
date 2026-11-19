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
    projektspezifische Header und deklariert die [!output APP_CLASS]-
    Anwendungsklasse.

[!output APP_IMPL]
    Dies ist die Hauptquelldatei der Anwendung, die die [!output APP_CLASS]-
    Anwendungsklasse enthält.

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
    Dies ist die Hauptressourcendatei des Projekts, die alle Microsoft Windows-
    Ressourcen auflistet, die das Projekt verwendet. Sie enthält die Symbole, 
    Bitmaps und Cursor, die im Unterverzeichnis "RES" gespeichert werden. Diese 
    Datei kann direkt in Microsoft Visual C++ bearbeitet werden. Ihre 
    Projektressourcen befinden sich in [!output LCID].
    Wenn die RC-Datei beibehalten wird, werden die Definitionen im 
    Datenabschnitt als Hexadezimalversion des numerischen Wertes, für den sie 
    definiert sind, gespeichert, und nicht als der angezeigte Name der 
    Definition.

res\[!output RC2_FILE_NAME].rc2
    Diese Datei enthält Ressourcen, die nicht von Microsoft Visual C++
    bearbeitet werden. Sie sollten alle Ressourcen, die nicht mit dem
    Ressourcen-Editor bearbeitet werden können, in dieser Datei platzieren.
[!endif]

[!if POCKETPC2003_UI_MODEL]
[!output PROJECT_NAME]ppc.rc
    Dies ist die Hauptressourcendatei des Projekts, die alle Microsoft Windows-
    Ressourcen aufführt, die das Projekt beim Kompilieren für die Pocket PC-
    Plattform oder eine Plattform, die das gleiche Benutzeroberflächenmodell 
    unterstützt, verwendet. Sie enthält die Symbole, Bitmaps und Cursor, die im 
    Unterverzeichnis "RES" gespeichert sind. Diese Datei kann direkt in 
    Microsoft Visual C++ bearbeitet werden. Ihre Projektressourcen befinden
    sich in [!output LCID]. Wenn die RC-Datei beibehalten wird, werden die 
    Definitionen im Datenabschnitt als Hexadezimalversion des numerischen 
    Wertes, für den sie definiert sind, gespeichert, und nicht als der 
    angezeigte Name der Definition.

res\[!output RC2_FILE_NAME]ppc.rc2
    Diese Datei enthält Ressourcen, die nicht von Microsoft Visual C++
    bearbeitet werden. Sie sollten alle Ressourcen, die nicht mit dem
    Ressourcen-Editor bearbeitet werden können, in dieser Datei platzieren.
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
[!output PROJECT_NAME]sp.rc
    Dies ist die Hauptressourcendatei des Projekts, die alle Microsoft Windows-
    Ressourcen aufführt, die das Projekt beim Kompilieren für die Smartphone-
    Plattform oder eine Plattform, die das gleiche Benutzeroberflächenmodell 
    unterstützt, verwendet. Sie enthält die Symbole, Bitmaps und Cursor, die im 
    Unterverzeichnis "RES" gespeichert sind. Diese Datei kann direkt in  
    Microsoft Visual C++ bearbeitet werden. Ihre Projektressourcen befinden
    sich in [!output LCID]. Wenn die RC-Datei beibehalten wird, werden die 
    Definitionen im Datenabschnitt als Hexadezimalversion des numerischen 
    Wertes, für den sie definiert sind, gespeichert, und nicht als der 
    angezeigte Name der Definition.

res\[!output RC2_FILE_NAME]sp.rc2
    Diese Datei enthält Ressourcen, die nicht von Microsoft Visual C++
    bearbeitet werden. Sie sollten alle Ressourcen, die nicht mit dem
    Ressourcen-Editor bearbeitet werden können, in dieser Datei platzieren.
[!endif]
[!if AYGSHELL_UI_MODEL]
[!output PROJECT_NAME]ayg.rc
    Dies ist die Hauptressourcendatei des Projekts, die alle Microsoft Windows-
    Ressourcen aufführt, die das Projekt beim Kompilieren für eine Plattform,
    die das AYGShell-Benutzeroberflächenmodell unterstützt, verwendet. Sie 
    enthält die Symbole, Bitmaps und Cursor, die im Unterverzeichnis "RES" 
    gespeichert werden. Diese Datei kann direkt in Microsoft Visual C++ 
    bearbeitet werden. Ihre Projektressourcen befinden sich in [!output LCID].
    Wenn die RC-Datei beibehalten wird, werden die Definitionen im 
    Datenabschnitt als Hexadezimalversion des numerischen Wertes, für den sie 
    definiert sind, gespeichert, und nicht als der angezeigte Name der 
    Definition.

res\[!output RC2_FILE_NAME]ayg.rc2
    Diese Datei enthält Ressourcen, die nicht von Microsoft Visual C++
    bearbeitet werden. Sie sollten alle Ressourcen, die nicht mit dem
    Ressourcen-Editor bearbeitet werden können, in dieser Datei platzieren.
[!endif]

res\[!output PROJECT_NAME].ico
    Dies ist eine Symboldatei, die als Symbol der Anwendung verwendet wird. 
    Dieses Symbol ist in der Hauptressourcendatei enthalten.

[!if HAS_SUFFIX]
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
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

Für das Hauptrahmenfenster:
    Das Projekt enthält eine standardmäßige MFC-Schnittstelle.

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    Diese Dateien enthalten die [!output MAIN_FRAME_CLASS]-Rahmenklasse, die von 
    CFrameWnd abgeleitet wird und alle SDI-Rahmenfunktionen steuert.

[!if CMD_BAR_MENUNBUTTON || CMD_BAR_BROWSERSTYLE]
res\Cmdbar.bmp
    Diese Bitmapdatei wird verwendet, um gekachelte Bilder für die Befehlsleiste 
    zu erstellen.
    Die ursprüngliche Befehlsleiste wird in der [!output MAIN_FRAME_CLASS]–
    Klasse erstellt. 
    Bearbeiten Sie diese cmdbar-Bitmap gemeinsam mit dem Array in
    [!output MAIN_FRAME_IMPL], um weitere Befehlsleisten-Schaltflächen
    hinzuzufügen.
[!endif]

/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
Der Anwendungs-Assistent erstellt einen Dokumenttyp und eine Ansicht:

[!output DOC_HEADER], [!output DOC_IMPL] – das Dokument
    Diese Dateien enthalten Ihre [!output DOC_CLASS]–Klasse. Bearbeiten Sie 
    diese Dateien, um spezielle Dokumentdaten hinzuzufügen und Dateispeicherung 
    und –ladung (über [!output DOC_CLASS]::Serialize) zu implementieren.
[!if HAS_SUFFIX]
    Das Dokument weist folgende Zeichenfolgen auf:
        Dateierweiterung:          [!output FILE_EXTENSION]
        Dateityp-ID:               [!output FILE_TYPE_ID]
        Hauptrahmenüberschrift:    [!output MAIN_FRAME_CAPTION]
        Dokumenttypname:           [!output DOC_TYPE_NAME]
        Filtername:                [!output FILTER_NAME]
        Neuer Dateikurzname:       [!output FILE_NEW_NAME_SHORT]
        Langer Name des Dateityps: [!output FILE_NEW_NAME_LONG]
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL] – die Ansicht des Dokuments
    Diese Dateien enthalten Ihre [!output VIEW_CLASS]–Klasse.
    [!output VIEW_CLASS]-Objekte werden verwendet, um [!output DOC_CLASS]-
    Objekte anzuzeigen.

[!endif]

[!else]
/////////////////////////////////////////////////////////////////////////////

Der Anwendungs-Assistent erstellt eine Dialogfeldklasse:

[!output DIALOG_HEADER], [!output DIALOG_IMPL] – das Dialogfeld
    Diese Dateien enthalten Ihre [!output DIALOG_CLASS]–Klasse. Diese Klasse 
    bestimmt das Verhalten des Hauptdialogfelds Ihrer Anwendung. Die Vorlage des 
    Dialogfelds befindet sich in der Hauptressourcendatei, die in Microsoft 
    Visual C++ bearbeitet werden kann.
[!endif]

[!if HELPSTYLE_WINHELP]
/////////////////////////////////////////////////////////////////////////////

Hilfeunterstützung:

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

[!if ACTIVEX_CONTROLS || PRINTING || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

Weitere Funktionen:
[!if APP_TYPE_DLG]
[!else]
[!if CMD_BAR_MENUONLY]

   Befehlsleiste im Hauptrahmen mit Menü und Zusatzelementen
[!else]
[!if CMD_BAR_MENUNBUTTON]
   Menüleiste im Hauptrahmen mit Menü, Schaltflächen und Zusatzelementen
[!endif]
[!endif]
[!if STATUS_BAR]
   Ursprüngliche Statusleiste im Hauptrahmen
[!endif]
[!endif]
[!if HELPSTYLE_WINHELP]

   Windows CE-Hilfeunterstützung
[!endif]
[!if DYNAMIC_MFC]

   Verwendet freigegebene DLL-Implementierungen
[!endif]
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
[!if SOCKETS]

Windows Sockets
    Die Anwendung unterstützt die Kommunikation über TCP/IP-Netzwerke.
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

Weitere Standarddateien:

StdAfx.h, StdAfx.cpp
    Diese Dateien werden verwendet, um eine vorkompilierte Headerdatei
    (PCH-Datei) mit dem Namen "[!output PROJECT_NAME].pch2 und eine 
    vorkompilierte Typendatei mit dem Namen "StdAfx.obj" zu erstellen.

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
    Dies ist die Standardheaderdatei, die neue Ressourcen-IDs definiert.
    Microsoft Visual C++ liest und aktualisiert diese Datei.

/////////////////////////////////////////////////////////////////////////////

Weitere Hinweise:

Der Anwendungs-Assistent verwendet "TODO:, um auf Teile des Quellcodes
hinzuweisen, die Sie hinzufügen oder anpassen sollten.
[!if APP_TYPE_SDI || APP_TYPE_DLG]

Wenn Ihre Anwendung MFC in einer freigegebenen DLL verwendet und Ihre Anwendung
in einer anderen als der aktuellen Sprache des Betriebssystems vorliegt,
müssen Sie die entsprechenden lokalisierten Ressourcen "MFC80XXX.DLL" in Ihr
Anwendungsverzeichnis kopieren ("XXX" steht für die Sprachabkürzung. "MFC80DEU.DLL" enthält beispielsweise ins Deutsche übersetzte Ressourcen.) Wenn Sie dies nicht tun, bleiben einige der Benutzeroberflächenelemente Ihrer Anwendung in der Sprache des Betriebssystems.
[!endif]

/////////////////////////////////////////////////////////////////////////////