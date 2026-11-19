================================================================================
    LIBRERIA MFC: cenni preliminari sul progetto [!output PROJECT_NAME]
===============================================================================

La creazione guidata applicazione ha creato l'applicazione 
[!output PROJECT_NAME].  Tale applicazione illustra le nozioni fondamentali sull'utilizzo delle classi 
MFC (Microsoft Foundation Class) e costituisce un punto di partenza per la scrittura dell'applicazione.

Questo file contiene un riepilogo del contenuto di ciascun file che fa parte
dell'applicazione [!output PROJECT_NAME].

[!output PROJECT_NAME].vcproj
    File di progetto principale per i progetti VC++ generati tramite una creazione guidata applicazione. 
    Contiene informazioni sulla versione di Visual C++ che ha generato il file e 
    informazioni sulle piattaforme, le configurazioni e le caratteristiche del 
    progetto selezionate con la creazione guidata applicazione.

[!output APP_HEADER]
    File di intestazione principale per l'applicazione.  Include altre
    intestazioni specifiche del progetto e dichiara la classe di applicazione [!output APP_CLASS].

[!output APP_IMPL]
    File di origine principale dell'applicazione contenente la classe di
    applicazione [!output APP_CLASS].

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
    File di risorse principale del progetto contenente l'elenco di tutte le 
    risorse Microsoft Windows utilizzate dal programma.  Include le icone, le bitmap 
    e i cursori memorizzati nella sottodirectory RES.  Questo file può essere modificato direttamente
    in Microsoft Visual C++. Le risorse del progetto sono in [!output LCID].
    Se il file rc è persistente, le definizioni nella sezione dati sono persistenti
    come la versione esadecimale del valore numerico per cui sono definite invece del 
    nome descrittivo della definizione.

res\[!output RC2_FILE_NAME].rc2
    Questo file contiene le risorse non modificate da Microsoft
    Visual C++. Inserire in questo file tutte le risorse non modificabili
    dall'editor di risorse.
[!endif]

[!if POCKETPC2003_UI_MODEL]
[!output PROJECT_NAME]ppc.rc
    File di risorse principale del progetto contenente l'elenco di tutte le 
    risorse Microsoft Windows utilizzate dal progetto durante la compilazione per la piattaforma Pocket PC o una
    piattaforma che supporti lo stesso modello di interfaccia utente.  Include le icone,
    le bitmap e i cursori memorizzati nella sottodirectory RES.  Questo file può essere 
    modificato direttamente in Microsoft Visual C++. Le risorse del progetto sono in 
    [!output LCID]. Se il file rc è persistente, le definizioni nella sezione
    di dati vengono mantenute come la versione esadecimale del valore numerico per cui sono definite
    invece del nome descrittivo della definizione.

res\[!output RC2_FILE_NAME]ppc.rc2
    Questo file contiene le risorse non modificate da Microsoft
    Visual C++. Inserire in questo file tutte le risorse non modificabili
    dall'editor di risorse.
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
[!output PROJECT_NAME]sp.rc
    File di risorse principale del progetto contenente l'elenco di tutte le 
    risorse Microsoft Windows utilizzate dal progetto durante la compilazione per la piattaforma Smartphone o una
    piattaforma che supporti lo stesso modello di interfaccia utente.  Include le icone,
    le bitmap e i cursori memorizzati nella sottodirectory RES.  Questo file può essere 
    modificato direttamente in Microsoft Visual C++. Le risorse del progetto sono in 
    [!output LCID]. Se il file rc è persistente, le definizioni nella sezione
    di dati vengono mantenute come la versione esadecimale del valore numerico per cui sono definite
    invece del nome descrittivo della definizione.

res\[!output RC2_FILE_NAME]sp.rc2
    Questo file contiene le risorse non modificate da Microsoft
    Visual C++. Inserire in questo file tutte le risorse non modificabili
    dall'editor di risorse.
[!endif]
[!if AYGSHELL_UI_MODEL]
[!output PROJECT_NAME]ayg.rc
    File di risorse principale del progetto contenente l'elenco di tutte le 
    risorse Microsoft Windows utilizzate dal progetto durante la compilazione 
    per una piattaforma che supporti il modello di interfaccia utente AYGShell.  Include le icone, le bitmap 
    e i cursori memorizzati nella sottodirectory RES.  Questo file può essere modificato direttamente
    in Microsoft Visual C++. Le risorse del progetto sono in [!output LCID].
    Se il file rc è persistente, le definizioni nella sezione dati sono persistenti
    come la versione esadecimale del valore numerico per cui sono definite invece del 
    nome descrittivo della definizione.

res\[!output RC2_FILE_NAME]ayg.rc2
    Questo file contiene le risorse non modificate da Microsoft
    Visual C++. Inserire in questo file tutte le risorse non modificabili
    dall'editor di risorse.
[!endif]

res\[!output PROJECT_NAME].ico
    File di icona utilizzato come icona dell'applicazione.  Tale
    icona è inclusa dal file di risorse principale.

[!if HAS_SUFFIX]
[!output PROJECT_NAME].reg
    File reg di esempio indicante il tipo di impostazioni di registrazione
    che verranno configurate dal framework.  È possibile utilizzarlo come file reg
[!if APP_TYPE_DLG]
    per il supporto dell'applicazione.
[!else]
    per il supporto dell'applicazione oppure eliminarlo per utilizzare
    la registrazione predefinita RegisterShellFileTypes.
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

Per la finestra frame principale:
    Il progetto include un'interfaccia MFC standard.

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    Questi file contengono la classe frame [!output MAIN_FRAME_CLASS] che deriva da
    CFrameWnd e controlla tutte le caratteristiche frame SDI.

[!if CMD_BAR_MENUNBUTTON || CMD_BAR_BROWSERSTYLE]
res\Cmdbar.bmp
    File bitmap utilizzato per creare immagini affiancate per la barra dei comandi.
    La barra dei comandi è costruita nella classe [!output INPLACE_FRAME_CLASS]. 
    Per aggiungere ulteriori pulsanti della barra dei comandi, modificare la bitmap
    cmdbar insieme alla matrice in [!output MAIN_FRAME_IMPL].
[!endif]

/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
La creazione guidata applicazione crea un tipo di documento e una visualizzazione:

[!output DOC_HEADER], [!output DOC_IMPL], il documento
    Questi file contengono la classe [!output DOC_CLASS].  Modificare questi file
    per aggiungere i dati speciali del documento e consentire il salvataggio
    e il caricamento del file tramite [!output DOC_CLASS]::Serialize.
[!if HAS_SUFFIX]
    Il documento contiene le seguenti stringhe:
        Estensione di file:      [!output FILE_EXTENSION]
        ID del tipo di file:        [!output FILE_TYPE_ID]
        Didascalia del frame principale:  [!output MAIN_FRAME_CAPTION]
        Nome del tipo di documento:       [!output DOC_TYPE_NAME]
        Nome del filtro:         [!output FILTER_NAME]
        Nuovo nome breve del file: [!output FILE_NEW_NAME_SHORT]
        Nome lungo del tipo di file: [!output FILE_NEW_NAME_LONG]
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL], la visualizzazione del documento
    Questi file contengono la classe [!output VIEW_CLASS].
    Per visualizzare gli oggetti [!output DOC_CLASS] vengono utilizzati oggetti [!output VIEW_CLASS].

[!endif]

[!else]
/////////////////////////////////////////////////////////////////////////////

La creazione guidata applicazione crea una classe di finestre di dialogo:

[!output DIALOG_HEADER], [!output DIALOG_IMPL], la finestra di dialogo
    Questi file contengono la classe [!output DIALOG_CLASS].  Tale classe definisce
    il comportamento della finestra di dialogo principale dell'applicazione.  Il modello della finestra
    di dialogo si trova nel file di risorse principale e può essere modificato in Microsoft Visual C++.
[!endif]

[!if HELPSTYLE_WINHELP]
/////////////////////////////////////////////////////////////////////////////

Supporto della Guida:

hlp\[!output PROJECT_NAME].hpj
    File di progetto della Guida utilizzato dal compilatore della Guida per creare
    la Guida relativa all'applicazione.

hlp\*.bmp
    File bitmap richiesti dagli argomenti del file della Guida standard per i
    comandi standard della libreria MFC.

hlp\*.rtf
    File contenenti gli argomenti della Guida per i comandi MFC e
    gli oggetti schermo standard.
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

Altre funzionalità:
[!if APP_TYPE_DLG]
[!else]
[!if CMD_BAR_MENUONLY]

   Barra dei comandi nel frame principale con menu ed elementi grafici
[!else]
[!if CMD_BAR_MENUNBUTTON]
   Barra dei menu nel frame principale con menu, pulsanti ed elementi grafici
[!endif]
[!endif]
[!if STATUS_BAR]
   Barra di stato iniziale nel frame principale
[!endif]
[!endif]
[!if HELPSTYLE_WINHELP]

   Supporto della Guida di Windows CE
[!endif]
[!if DYNAMIC_MFC]

   Utilizza l'implementazione della DLL condivisa
[!endif]
[!if ACTIVEX_CONTROLS]

Controlli ActiveX
    L'applicazione include il supporto per utilizzare i controlli ActiveX.
[!endif]
[!if PRINTING]

Supporto stampa e anteprima di stampa
    La creazione guidata applicazione ha generato il codice per gestire i comandi Stampa, Imposta stampante
    e Anteprima di stampa tramite la chiamata alle funzioni membro nella classe CView dalla libreria MFC.
[!endif]
[!if SOCKETS]

Windows Sockets
    L'applicazione contiene il supporto per stabilire le comunicazioni su reti TCP/IP.
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

Altri file standard:

StdAfx.h, StdAfx.cpp
    Tali file vengono utilizzati per generare il file di intestazione
    precompilato [!output PROJECT_NAME].pch e il file dei tipi precompilato StdAfx.obj.

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
    File di intestazione principale standard che definisce i nuovi ID risorse.
    Tale file viene letto e aggiornato da Microsoft Visual C++.

/////////////////////////////////////////////////////////////////////////////

Altre note:

la creazione guidata applicazione utilizza il prefisso "TODO:" per indicare le parti del codice sorgente
da aggiungere o personalizzare.
[!if APP_TYPE_SDI || APP_TYPE_DLG]

Se l'applicazione utilizza MFC in una DLL condivisa ed è in una lingua diversa 
da quella corrente del sistema operativo, è necessario copiare il file 
MFC80XXX.DLL corrispondete delle risorse localizzate nella directory dell'applicazione ("XXX" indica l'abbreviazione della lingua,  ad esempio,
MFC80DEU.DLL contiene le risorse tradotte in tedesco).  In caso contrario,
alcuni elementi dell'interfaccia utente dell'applicazione rimarranno nella
stessa lingua del sistema operativo.
[!endif]

/////////////////////////////////////////////////////////////////////////////