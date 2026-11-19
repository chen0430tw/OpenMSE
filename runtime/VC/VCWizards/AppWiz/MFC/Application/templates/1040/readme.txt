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
    intestazioni specifiche del progetto quale Resource.h e dichiara la
    classe di applicazione [!output APP_CLASS].

[!output APP_IMPL]
    File di origine principale dell'applicazione contenente la classe di
    applicazione [!output APP_CLASS].

[!output PROJECT_NAME].rc
    Elenco di tutte le risorse Microsoft Windows utilizzate dal
    programma.  Include le icone, le bitmap e i cursori memorizzati
    nella sottodirectory RES.  Questo file può essere modificato direttamente
    in Microsoft Visual C++. Le risorse del progetto sono in [!output LCID].

res\[!output PROJECT_NAME].ico
    File di icona utilizzato come icona dell'applicazione.  Tale
    icona è inclusa dal file di risorse principale [!output PROJECT_NAME].rc.

res\[!output RC2_FILE_NAME].rc2
    Questo file contiene le risorse non modificate da Microsoft
    Visual C++. Inserire in questo file tutte le risorse non modificabili
    dall'editor di risorse.
[!if CONTAINER_SERVER || FULL_SERVER || MINI_SERVER || AUTOMATION || HAS_SUFFIX]
[!if !HTML_EDITVIEW]

[!output PROJECT_NAME].reg
    File reg di esempio indicante il tipo di impostazioni di registrazione
    che verranno configurate dal framework.  È possibile utilizzarlo come file reg
[!if APP_TYPE_DLG]
    per il supporto dell'applicazione.
[!else]
    per il supporto dell'applicazione oppure eliminarlo per utilizzare
    la registrazione predefinita RegisterShellFileTypes.
[!endif]

[!if AUTOMATION]
[!output SAFE_IDL_NAME].idl
    File contenente il codice sorgente IDL per la libreria dei tipi
    dell'applicazione.
[!endif]
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

Per la finestra frame principale:
[!if PROJECT_STYLE_EXPLORER]
    Stile Esplora risorse: il progetto include un'interfaccia tipo Esplora
    risorse con due frame.
[!else]
    Il progetto include un'interfaccia MFC standard.
[!endif]

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    Questi file contengono la classe frame [!output MAIN_FRAME_CLASS] che deriva da
[!if APP_TYPE_MDI]
    CMDIFrameWnd e controlla tutte le funzionalità frame MDI.
[!else]
    CFrameWnd e controlla tutte le funzionalità frame SDI.
[!endif]
[!if PROJECT_STYLE_EXPLORER]

[!output TREE_VIEW_HEADER], [!output TREE_VIEW_IMPL]
    Questi file contengono la classe frame di sinistra [!output TREE_VIEW_CLASS] che deriva da
    CTreeView.
[!endif]

[!if DOCKING_TOOLBAR]
res\Toolbar.bmp
    File bitmap utilizzato per creare immagini affiancate per la barra degli strumenti.
    La barra degli strumenti e la barra di stato sono inizialmente costruite nella classe
    [!output MAIN_FRAME_CLASS]. Per aggiungere i pulsanti della barra degli strumenti,
    modificare questa bitmap della barra degli strumenti utilizzando l'editor
    risorse e aggiornare la matrice IDR_MAINFRAME TOOLBAR in [!output PROJECT_NAME].rc.
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

res\IToolbar.bmp
    File bitmap utilizzato per creare immagini affiancate per la barra
    degli strumenti quando l'applicazione server è attivata sul posto
    in un altro contenitore. La barra degli strumenti è costruita nella classe
    [!output INPLACE_FRAME_CLASS]. Questa bitmap è simile a quella presente in res\Toolbar.bmp,
    con molti comandi non relativi al server rimossi.

[!endif]
[!endif]
[!if APP_TYPE_MDI]
/////////////////////////////////////////////////////////////////////////////

Per la finestra frame figlio:

ChildFrm.h, ChildFrm.cpp
    Questi file definiscono e implementano la classe [!output CHILD_FRAME_CLASS] che
    supporta le finestre figlio in un'applicazione MDI.

[!endif]
/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
[!if !DB_VIEW_NO_FILE]
La creazione guidata applicazione crea un tipo di documento e una visualizzazione:

[!output DOC_HEADER], [!output DOC_IMPL], il documento
    Questi file contengono la classe [!output DOC_CLASS].  Modificare questi file per
    aggiungere i dati speciali del documento e consentire il salvataggio e il
    caricamento del file tramite [!output DOC_CLASS]::Serialize.
[!if HAS_SUFFIX && !HTML_EDITVIEW]
    Il documento contiene le seguenti stringhe:
        Estensione di file:      [!output FILE_EXTENSION]
        ID del tipo di file:        [!output FILE_TYPE_ID]
        Didascalia del frame principale:  [!output MAIN_FRAME_CAPTION]
        Nome del tipo di documento:       [!output DOC_TYPE_NAME]
        Nome del filtro:         [!output FILTER_NAME]
        Nuovo nome breve del file: [!output FILE_NEW_NAME_SHORT]
        Nome lungo del tipo di file: [!output FILE_NEW_NAME_LONG]
[!endif]
[!else]
La creazione guidata applicazione crea una visualizzazione:
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL], la visualizzazione del documento
    Questi file contengono la classe [!output VIEW_CLASS].
[!if !DB_VIEW_NO_FILE]
    Per visualizzare gli oggetti [!output DOC_CLASS] vengono utilizzati oggetti [!output VIEW_CLASS].
[!endif]

[!if APP_TYPE_MDI]
res\[!output PROJECT_NAME]Doc.ico
    File di icona utilizzato come icona per le finestre figlio MDI per la
    classe [!output DOC_CLASS].  Tale icona è inclusa dal file di
    risorse principale [!output PROJECT_NAME].rc.
[!endif]

[!endif]

[!if DB_VIEW_NO_FILE || DB_VIEW_WITH_FILE]
/////////////////////////////////////////////////////////////////////////////

Supporto database:

[!output ROWSET_HEADER], [!output ROWSET_IMPL]
    Questi file contengono la classe [!output ROWSET_CLASS].  Tale classe viene utilizzata
    per accedere all'origine dati selezionata nella procedura guidata.
[!if DB_VIEW_NO_FILE]
    Non verrà aggiunto alcun supporto per la serializzazione.
[!else]
    È stato aggiunto il supporto per la serializzazione.
[!endif]
[!endif]
[!if CONTAINER || FULL_SERVER || MINI_SERVER || CONTAINER_SERVER]
/////////////////////////////////////////////////////////////////////////////

La creazione guidata applicazione crea inoltre le classi specifiche per OLE.

[!if CONTAINER || CONTAINER_SERVER]
[!output CONTAINER_ITEM_HEADER], [!output CONTAINER_ITEM_IMPL]
    Questi file contengono la classe [!output CONTAINER_ITEM_CLASS]. Tale classe viene utilizzata 
    per modificare gli oggetti OLE.  Gli oggetti OLE sono normalmente visualizzati
    dalla classe [!output VIEW_CLASS] e serializzati come parte della classe [!output DOC_CLASS].
[!if ACTIVE_DOC_CONTAINER]
    Il programma include il supporto per contenere documenti attivi nel frame.
[!endif]
[!endif]
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

[!output SERVER_ITEM_HEADER], [!output SERVER_ITEM_IMPL]
    Questi file contengono la classe [!output SERVER_ITEM_CLASS]. Tale classe viene utilizzata
    per connettere la classe [!output DOC_CLASS] al sistema OLE e
    facoltativamente fornire collegamenti al documento.
[!if ACTIVE_DOC_SERVER]
    Il progetto contiene il supporto per creare e gestire documenti attivi.
[!endif]

[!output INPLACE_FRAME_HEADER], [!output INPLACE_FRAME_IMPL]
    Questi file contengono la classe [!output INPLACE_FRAME_CLASS]. Tale classe è derivata
    da COleIPFrameWnd e controlla tutte le funzionalità di frame durante l'attivazione sul posto.
[!endif]

[!if SUPPORT_COMPOUND_FILES]
    Il progetto contiene il supporto per i file compositi. Il formato di file composito consente di memorizzare documenti 
    che contengono uno o più oggetti di automazione per un file e consente l'accesso per 
    i singoli oggetti.
[!endif]
[!endif]
[!else]

/////////////////////////////////////////////////////////////////////////////

[!if AUTOMATION]
La creazione guidata applicazione crea una classe di finestre di dialogo e una classe proxy di automazione:
[!else]
La creazione guidata applicazione crea una classe di finestre di dialogo:
[!endif]

[!output DIALOG_HEADER], [!output DIALOG_IMPL], la finestra di dialogo
    Questi file contengono la classe [!output DIALOG_CLASS].  Tale classe definisce
    il comportamento della finestra di dialogo principale dell'applicazione.  Il modello della finestra di
    dialogo si trova in [!output PROJECT_NAME].rc e può essere modificato in Microsoft Visual C++.
[!if AUTOMATION]

[!output DIALOG_AUTO_PROXY_HEADER], [!output DIALOG_AUTO_PROXY_IMPL], l'oggetto di automazione
    Questi file contengono la classe [!output DIALOG_AUTO_PROXY_CLASS].  Tale classe
    è denominata classe proxy di automazione per la finestra di dialogo perché
    gestisce l'esposizione dei metodi e delle proprietà di automazione
    utilizzati dai controller di automazione per accedere alla finestra di dialogo.  I metodi
    e le proprietà non sono esposte direttamente dalla classe di finestre di
    dialogo perché, nel caso di un'applicazione MFC basata su finestra di
    dialogo modale, è più semplice e corretto mantenere l'oggetto di automazione separato dall'interfaccia utente.
[!endif]
[!endif]

[!if CONTEXT_HELP]
/////////////////////////////////////////////////////////////////////////////

Supporto della Guida:

[!if HELPSTYLE_HTML]
hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhp
    File di progetto della Guida. Contiene i dati necessari per
    compilare i file della Guida in un file chm.

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhc
    File in cui viene elencato il contenuto del progetto della Guida.

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhk
    File contenente l'indice degli argomenti della Guida.

hlp\afxcore.htm
    File contenente gli argomenti della Guida per i comandi MFC
    e gli oggetti schermo standard. Gli argomenti della Guida si aggiungono in questo file.

[!if PRINTING]
hlp\afxprint.htm
    File contenente gli argomenti della Guida per i comandi di stampa.

[!endif]
makehtmlhelp.bat
    File utilizzato dal sistema di generazione per compilare i file della Guida.

hlp\Images\*.gif
    File bitmap richiesti dagli argomenti del file della Guida standard per
    i comandi standard della libreria MFC.

[!else]
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
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SPLITTER || MAPI || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

Altre funzionalità:
[!if ACTIVEX_CONTROLS]

Controlli ActiveX
    L'applicazione include il supporto per utilizzare i controlli ActiveX.
[!endif]
[!if PRINTING]

Supporto stampa e anteprima di stampa
    La creazione guidata applicazione ha generato il codice per gestire i comandi Stampa, Imposta stampante
    e Anteprima di stampa tramite la chiamata alle funzioni membro nella classe CView dalla libreria MFC.
[!endif]
[!if DB_SUPPORT_HEADER_ONLY && !APP_TYPE_DLG]

Supporto database
    La creazione guidata applicazione ha aggiunto il livello di base del supporto database per il programma. 
    Sono stati inclusi solo i file necessari.
[!endif]
[!if SPLITTER && !APP_TYPE_DLG]

Finestra divisa
    La creazione guidata applicazione ha aggiunto il supporto per le finestre divise per i documenti dell'applicazione.
[!endif]
[!if MAPI]

Supporto MAPI
    Il progetto generato contiene il codice necessario per creare, modificare, trasferire e
    memorizzare i messaggi di posta elettronica.
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

Resource.h
    File di intestazione principale standard che definisce i nuovi ID risorse.
    Tale file viene letto e aggiornato da Microsoft Visual C++.

[!if MANIFEST]
[!output PROJECT_NAME].manifest
	I file manifesto delle applicazioni vengono utilizzati da Windows XP per 
	descrivere una dipendenza delle applicazioni per versioni specifiche di assembly affiancati. Queste informazioni 
	vengono utilizzate per il caricamento dell'assembly appropriato dalla cache o privatamente 
	dall'applicazione. Il manifesto dell'applicazione potrebbe essere incluso per la ridistribuzione 
	come un file manifest esterno installato nella stessa cartella dell'eseguibile 
	dell'applicazione o potrebbe essere compreso nell'eseguibile sotto forma di risorsa. 
[!endif]
/////////////////////////////////////////////////////////////////////////////

Altre note:

la creazione guidata applicazione utilizza il prefisso "TODO:" per indicare le parti del
codice sorgente da aggiungere o personalizzare.
[!if APP_TYPE_MDI || APP_TYPE_SDI || APP_TYPE_DLG || APP_TYPE_MTLD]

Se l’applicazione utilizza MFC in una DLL condivisa, sarà necessario 
ridistribuire le DLL MFC. Se l’applicazione è in una lingua diversa 
da quella del sistema operativo, sarà inoltre necessario 
ridistribuire il file MFC80XXX.DLL delle risorse corrispondenti localizzate. 
Per ulteriori informazioni su entrambi gli argomenti, vedere la sezione relativa 
alla ridistribuzione di applicazioni Visual C++ nella documentazione MSDN.
[!endif]

/////////////////////////////////////////////////////////////////////////////