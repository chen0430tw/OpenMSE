================================================================================
    BIBLIOTHÈQUE MICROSOFT FOUNDATION CLASS : Vue d'ensemble du projet [!output 
PROJECT_NAME]
===============================================================================

L'Assistant Application a créé cette application [!output PROJECT_NAME] pour  
vous.  Cette application décrit les principes de base de l'utilisation de  
Microsoft Foundation Classes et vous permet de créer votre application.

Ce fichier contient un résumé du contenu de chacun des fichiers qui 
constituent votre application [!output PROJECT_NAME].

[!output PROJECT_NAME].vcproj
    Il s'agit du fichier projet principal pour les projets VC++ générés à l'aide 
d'un Assistant Application. 
    Il contient les informations sur la version de Visual C++ qui a généré le 
fichier et  
    des informations sur les plates-formes, configurations et fonctionnalités du 
projet sélectionnées avec 
    l'Assistant Application.

[!output APP_HEADER]
    Il s'agit du fichier d'en-tête principal de l'application.  Il contient 
d'autres 
    en-têtes de projet spécifiques (y compris Resource.h) et déclare la 
    classe d'application [!output APP_CLASS].

[!output APP_IMPL]
    Il s'agit du fichier source principal de l'application qui contient la 
    classe d'application [!output APP_CLASS].

[!output PROJECT_NAME].rc
    Il s'agit de la liste de toutes les ressources Microsoft Windows que le 
    programme utilise.  Elle comprend les icônes, les bitmaps et les curseurs 
qui sont stockés 
    dans le sous-répertoire RES.  Ce fichier peut être modifié directement dans 
Microsoft 
    Visual C++. Vos ressources de projet sont dans [!output LCID].

res\[!output PROJECT_NAME].ico
    Il s'agit d'un fichier icône, qui est utilisé comme icône de l'application.  
Cette 
    icône est incluse par le fichier de ressource principal [!output 
PROJECT_NAME].rc.

res\[!output RC2_FILE_NAME].rc2
    Ce fichier contient les ressources qui ne sont pas modifiées par Microsoft  
    Visual C++. Vous devez placer toutes les ressources non modifiables par 
    l'éditeur de ressources dans ce fichier.
[!if CONTAINER_SERVER || FULL_SERVER || MINI_SERVER || AUTOMATION || HAS_SUFFIX]
[!if !HTML_EDITVIEW]

[!output PROJECT_NAME].reg
    Il s'agit d'un exemple de fichier .reg qui montre le type de paramètres 
    d'enregistrement que le framework définit pour vous.  Vous pouvez l'utiliser 
comme fichier .reg
[!if APP_TYPE_DLG]
    pour votre application.
[!else]
    pour votre application ou le supprimer et utiliser  
    l'enregistrement par défaut RegisterShellFileTypes.
[!endif]

[!if AUTOMATION]
[!output SAFE_IDL_NAME].idl
    Ce fichier contient le code source IDL (Interface Description Language) de  
    la bibliothèque de types de votre application.
[!endif]
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

Pour la fenêtre frame principale :
[!if PROJECT_STYLE_EXPLORER]
    Style de Windows Explorer : Le projet contiendra une interface de type 
Windows Explorer 
    avec deux frames.
[!else]
    Le projet comprend une interface MFC standard.
[!endif]

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    Ces fichiers contiennent la classe de frame [!output MAIN_FRAME_CLASS] 
dérivée de
[!if APP_TYPE_MDI]
    CMDIFrameWnd et qui contrôle toutes les fonctionnalités des frames MDI.
[!else]
    CFrameWnd et qui contrôle toutes les fonctionnalités des frames SDI.
[!endif]
[!if PROJECT_STYLE_EXPLORER]

[!output TREE_VIEW_HEADER], [!output TREE_VIEW_IMPL]
    Ces fichiers contiennent la classe de frame gauche [!output TREE_VIEW_CLASS] 
dérivée de
    CTreeView.
[!endif]

[!if DOCKING_TOOLBAR]
res\Toolbar.bmp
    Ce fichier bitmap sert à créer des images en mosaïque pour la barre 
d'outils.
    La barre d'outils et la barre d'état initiales sont construites dans la 
classe  
[!output MAIN_FRAME_CLASS]. Modifiez cette image de barre d'outils avec 
l'éditeur de ressources et 
    actualisez le tableau IDR_MAINFRAME TOOLBAR dans [!output PROJECT_NAME].rc 
pour ajouter 
    les boutons de barre d'outils.
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

res\IToolbar.bmp
    Ce fichier bitmap sert à créer des images en mosaïque pour la barre d'outils 
    quand votre application serveur est activée sur place dans un autre 
    conteneur. Cette barre d'outils est construite dans la classe [!output 
INPLACE_FRAME_CLASS]. Cette bitmap est semblable à celle qui se trouve dans 
res\Toolbar.bmp 
    mais ne contient pas les commandes non dédiées au serveur.

[!endif]
[!endif]
[!if APP_TYPE_MDI]
/////////////////////////////////////////////////////////////////////////////

Pour la fenêtre frame enfant :

ChildFrm.h, ChildFrm.cpp
    Ces fichiers définissent et implémentent la classe [!output 
CHILD_FRAME_CLASS] 
    qui prend en charge les fenêtres enfants d'une application MDI.

[!endif]
/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
[!if !DB_VIEW_NO_FILE]
L'Assistant Application crée un type de document et une vue :

[!output DOC_HEADER], [!output DOC_IMPL] - le document
    Ces fichiers contiennent votre classe [!output DOC_CLASS].  Modifiez ces 
fichiers pour 
    ajouter les données de document spéciales et implémenter l'enregistrement et 
le 
    chargement des fichiers (via [!output DOC_CLASS]::Serialize).
[!if HAS_SUFFIX && !HTML_EDITVIEW]
    Le document contiendra les chaînes suivantes :
        Extension de fichier :      [!output FILE_EXTENSION]
        ID du type de fichier :        [!output FILE_TYPE_ID]
        Titre du frame principal :  [!output MAIN_FRAME_CAPTION]
        Nom du type de document :       [!output DOC_TYPE_NAME]
        Nom de filtre :         [!output FILTER_NAME]
        Nom court de nouveau fichier : [!output FILE_NEW_NAME_SHORT]
        Nom long du type de fichier : [!output FILE_NEW_NAME_LONG]
[!endif]
[!else]
L'Assistant Application crée une vue :
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL] - la vue du document
    Ces fichiers contiennent votre classe [!output VIEW_CLASS].
[!if !DB_VIEW_NO_FILE]
    Les objets [!output VIEW_CLASS] servent à afficher les objets [!output 
DOC_CLASS].
[!endif]

[!if APP_TYPE_MDI]
res\[!output PROJECT_NAME]Doc.ico
    Il s'agit d'un fichier icône, qui est utilisé comme icône des fenêtres 
enfants MDI 
    pour la classe [!output DOC_CLASS].  Cette icône est incluse par le fichier 
de 
    ressources principal [!output PROJECT_NAME].rc.
[!endif]

[!endif]

[!if DB_VIEW_NO_FILE || DB_VIEW_WITH_FILE]
/////////////////////////////////////////////////////////////////////////////

Prise en charge des bases de données :

[!output ROWSET_HEADER], [!output ROWSET_IMPL]
    Ces fichiers contiennent votre classe [!output ROWSET_CLASS].  Cette classe 
sert à accéder à 
    la source de données sélectionnée dans l'Assistant.
[!if DB_VIEW_NO_FILE]
    La sérialisation ne sera pas prise en charge.
[!else]
    La sérialisation est prise en charge.
[!endif]
[!endif]
[!if CONTAINER || FULL_SERVER || MINI_SERVER || CONTAINER_SERVER]
/////////////////////////////////////////////////////////////////////////////

L'Assistant Application a également créé des classes spécifiques à OLE

[!if CONTAINER || CONTAINER_SERVER]
[!output CONTAINER_ITEM_HEADER], [!output CONTAINER_ITEM_IMPL]
    Ces fichiers contiennent votre classe [!output CONTAINER_ITEM_CLASS]. Cette 
classe sert à  
    manipuler les objets OLE.  Les objets OLE sont habituellement affichés 
    par votre classe [!output VIEW_CLASS] et sérialisés avec votre classe 
[!output DOC_CLASS].
[!if ACTIVE_DOC_CONTAINER]
    Le programme prend en charge l'intégration des documents actifs dans son 
frame.
[!endif]
[!endif]
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

[!output SERVER_ITEM_HEADER], [!output SERVER_ITEM_IMPL]
    Ces fichiers contiennent votre classe [!output SERVER_ITEM_CLASS]. Cette 
classe sert à 
    connecter votre classe [!output DOC_CLASS] au système OLE et, 
    éventuellement, à fournir des liens vers votre document.
[!if ACTIVE_DOC_SERVER]
    Le projet permet de créer et de gérer des documents actifs.
[!endif]

[!output INPLACE_FRAME_HEADER], [!output INPLACE_FRAME_IMPL]
    Ces fichiers contiennent votre classe [!output INPLACE_FRAME_CLASS]. Cette 
classe est dérivée de 
    COleIPFrameWnd et contrôle toutes les fonctionnalités de frame au cours de 
l'activation sur place.
[!endif]

[!if SUPPORT_COMPOUND_FILES]
    Le projet prend en charge les fichiers composés. Le format de fichier 
composé permet de stocker un document  
    qui contient un ou plusieurs objets Automation dans un fichier, tout en 
laissant  
    accessible chacun de ces objets.
[!endif]
[!endif]
[!else]

/////////////////////////////////////////////////////////////////////////////

[!if AUTOMATION]
L'Assistant Application crée une classe de boîte de dialogue et une classe proxy 
Automation :
[!else]
L'Assistant Application crée une classe de boîte de dialogue :
[!endif]

[!output DIALOG_HEADER], [!output DIALOG_IMPL] – la boîte de dialogue
    Ces fichiers contiennent votre classe [!output DIALOG_CLASS].  Cette classe 
définit 
    le comportement de la boîte de dialogue principale de votre application.  Le 
modèle de boîte de dialogue se trouve 
    dans [!output PROJECT_NAME].rc et peut être modifié dans Microsoft Visual 
C++.
[!if AUTOMATION]

[!output DIALOG_AUTO_PROXY_HEADER], [!output DIALOG_AUTO_PROXY_IMPL] - l'objet 
Automation
    Ces fichiers contiennent votre classe [!output DIALOG_AUTO_PROXY_CLASS].  
Cette classe 
    est appelée classe proxy Automation pour votre boîte de dialogue, car elle 
    protège les méthodes et les propriétés Automation que les contrôleurs 
    Automation peuvent utiliser pour accéder à votre boîte de dialogue.  Ces 
méthodes 
    et propriétés ne sont pas directement exposées par la classe de boîte de 
dialogue 
    car, dans le cas d'une application MFC modale basée sur des boîtes de 
dialogue, il est 
    plus simple de séparer l'objet Automation de l'interface utilisateur.
[!endif]
[!endif]

[!if CONTEXT_HELP]
/////////////////////////////////////////////////////////////////////////////

Prise en charge de l'aide :

[!if HELPSTYLE_HTML]
hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhp
    Ce fichier est un fichier projet d'aide. Il contient les données requises 
    pour compiler les fichiers d'aide dans un fichier .chm.

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhc
    Ce fichier répertorie le contenu du projet d'aide.

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhk
    Ce fichier contient un index des rubriques d'aide.

hlp\afxcore.htm
    Ce fichier contient les rubriques d'aide standard relatives aux 
    objets de l'écran et aux commandes MFC standard. Ajoutez à ce fichier vos 
propres rubriques d'aide.

[!if PRINTING]
hlp\afxprint.htm
    Ce fichier contient les rubriques d'aide relatives aux commandes 
d'impression.

[!endif]
makehtmlhelp.bat
    Ce fichier est utilisé par le système de génération pour compiler les 
fichiers d'aide.

hlp\Images\*.gif
    Il s'agit de fichiers bitmap requis par le fichier des rubriques d'aide 
standard pour 
    les commandes standard de la bibliothèque Microsoft Foundation Class.

[!else]
hlp\[!output PROJECT_NAME].hpj
    Il s'agit du fichier d'aide du projet utilisé par le compilateur d'aide pour 
créer 
    votre fichier d'aide de l'application.

hlp\*.bmp
    Il s'agit de fichiers bitmap requis par le fichier des rubriques d'aide 
standard pour 
    les commandes standard de la bibliothèque Microsoft Foundation Class.

hlp\*.rtf
    Ces fichiers contiennent les rubriques d'aide standard pour les 
    objets de l'écran et commandes MFC standard.
[!endif]
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SPLITTER || MAPI || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

Autres fonctionnalités :
[!if ACTIVEX_CONTROLS]

Contrôles ActiveX
    L'application comprend la prise en charge des contrôles ActiveX.
[!endif]
[!if PRINTING]

Prise en charge de l'impression et de l'aperçu avant impression
    L'Assistant Application a généré un code pour gérer les commandes 
d'impression, de configuration de l'impression et d'aperçu avant impression en 
appelant les 
     fonctions membres dans la classe CView, à partir de la bibliothèque MFC.
[!endif]
[!if DB_SUPPORT_HEADER_ONLY && !APP_TYPE_DLG]

Prise en charge des bases de données
    L'Assistant Application a ajouté le niveau de base de la prise en charge des 
bases de données pour votre programme. 
    Seuls les fichiers nécessaires sont inclus.
[!endif]
[!if SPLITTER && !APP_TYPE_DLG]

Fenêtre fractionnée
    L'Assistant Application a ajouté la prise en charge des fenêtre fractionnées 
pour les documents de votre application.
[!endif]
[!if MAPI]

Prise en charge de MAPI
    Le projet créé contient le code nécessaire pour créer, manipuler, transférer 
et 
    stocker les messages électroniques.
[!endif]
[!if SOCKETS]

Windows Sockets
    L'application peut prendre en charge l'établissement des communications dans 
les réseaux TCP/IP.
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

Autres fichiers standard :

StdAfx.h, StdAfx.cpp
    Ces fichiers sont utilisés pour générer un fichier d'en-tête précompilé 
(PCH) 
    nommé [!output PROJECT_NAME].pch et un fichier de type précompilé nommé 
Stdafx.obj.

Resource.h
    Il s'agit du ficher d'en-tête standard, qui définit les nouveaux ID de 
ressources.
    Microsoft Visual C++ lit et met à jour ce fichier.

[!if MANIFEST]
[!output PROJECT_NAME].manifest
	Les fichiers manifestes d'application sont utilisés par Windows XP pour 
décrire les dépendances des applications  
	sur des versions spécifiques des assemblys côte à côte. Le chargeur 
utilise ces  
	informations pour charger l'assembly approprié à partir du cache de 
l'assembly ou directement  
	à partir de l'application. Le manifeste de l'application peut être inclus 
pour redistribution  
	comme fichier .manifest externe installé dans le même dossier que 
l'exécutable de l'application  
	ou être inclus dans l'exécutable sous la forme d'une ressource. 
[!endif]
/////////////////////////////////////////////////////////////////////////////

Autres remarques :

L'Assistant Application utilise "TODO:" pour indiquer les parties du code source 
où 
vous devrez ajouter ou modifier du code.
[!if APP_TYPE_MDI || APP_TYPE_SDI || APP_TYPE_DLG || APP_TYPE_MTLD]

Si votre application utilise les MFC dans une DLL partagée vous devez redistribuer 
les DLL MFC. Si la langue de votre application n'est pas celle du système 
d'exploitation, vous devez également redistribuer le fichier des  ressources 
localisées MFC80XXX.DLL. Pour plus d'informations, consultez la section relative 
à la redistribution des applications Visual C++ dans la documentation MSDN.
[!endif]

/////////////////////////////////////////////////////////////////////////////
