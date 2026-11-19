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
    en-têtes de projet spécifiques et déclare la classe d'application [!output 
APP_CLASS].

[!output APP_IMPL]
    Il s'agit du fichier source principal de l'application qui contient la 
    classe d'application [!output APP_CLASS].

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
    Il s'agit du fichier de ressources principal du projet qui répertorie toutes 
les ressources Microsoft Windows que le projet utilise.  Il comprend les icônes, 
les bitmaps et les curseurs qui sont stockés dans le sous-répertoire RES.  Ce 
fichier peut être modifié directement 
    dans Microsoft Visual C++. Vos ressources de projet sont dans [!output 
LCID].
    Quand le fichier .rc est persistant, les définitions de la section des 
données sont enregistrées 
    comme versions hexadécimales des valeurs numériques qui les définissent au 
lieu  
    du nom convivial de la définition.

res\[!output RC2_FILE_NAME].rc2
    Ce fichier contient les ressources qui ne sont pas modifiées par Microsoft  
    Visual C++. Vous devez placer toutes les ressources non modifiables par 
    l'éditeur de ressources dans ce fichier.
[!endif]

[!if POCKETPC2003_UI_MODEL]
[!output PROJECT_NAME]ppc.rc
    Il s'agit du fichier de ressources principal qui répertorie toutes les 
ressources Microsoft Windows 
    que le projet utilise lors de la compilation pour la plate-forme Pocket PC 
ou une 
    plate-forme qui prend en charge le même modèle d'interface utilisateur.  Il 
comprend les icônes 
    les bitmaps et les curseurs qui sont stockés dans le sous-répertoire RES.  
Ce fichier peut être  
modifié directement dans Microsoft Visual C++. Vos ressources de projet sont 
dans 
    [!output LCID]. Quand le fichier .rc est persistant, les définitions de la 
section des données sont enregistrées 
    comme versions hexadécimales des valeurs numériques qui les définissent au 
lieu  
    du nom convivial de la définition.

res\[!output RC2_FILE_NAME]ppc.rc2
    Ce fichier contient les ressources qui ne sont pas modifiées par Microsoft  
    Visual C++. Vous devez placer toutes les ressources non modifiables par 
    l'éditeur de ressources dans ce fichier.
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
[!output PROJECT_NAME]sp.rc
    Il s'agit du fichier de ressources principal qui répertorie toutes les 
ressources Microsoft Windows 
    que le projet utilise lors de la compilation pour la plate-forme Smartphone 
ou une 
    plate-forme qui prend en charge le même modèle d'interface utilisateur.  Il 
comprend les icônes 
    les bitmaps et les curseurs qui sont stockés dans le sous-répertoire RES.  
Ce fichier peut être  
modifié directement dans Microsoft Visual C++. Vos ressources de projet sont 
dans 
    [!output LCID]. Quand le fichier .rc est persistant, les définitions de la 
section des données sont enregistrées 
    comme versions hexadécimales des valeurs numériques qui les définissent au 
lieu  
    du nom convivial de la définition.

res\[!output RC2_FILE_NAME]sp.rc2
    Ce fichier contient les ressources qui ne sont pas modifiées par Microsoft  
    Visual C++. Vous devez placer toutes les ressources non modifiables par 
    l'éditeur de ressources dans ce fichier.
[!endif]
[!if AYGSHELL_UI_MODEL]
[!output PROJECT_NAME]ayg.rc
    Il s'agit du fichier de ressources principal qui répertorie toutes les 
ressources Microsoft Windows 
    que le projet utilise lors de la compilation pour une plate-forme qui 
    prend en charge le modèle d'interface utilisateur AYGShell.  Il comprend les 
icônes, les bitmaps,  
    et les curseurs qui sont stockés dans le sous-répertoire RES.  Ce fichier 
peut être modifié directement 
    dans Microsoft Visual C++. Vos ressources de projet sont dans [!output 
LCID].
    Quand le fichier .rc est persistant, les définitions de la section des 
données sont enregistrées 
    comme versions hexadécimales des valeurs numériques qui les définissent au 
lieu  
    du nom convivial de la définition.

res\[!output RC2_FILE_NAME]ayg.rc2
    Ce fichier contient les ressources qui ne sont pas modifiées par Microsoft  
    Visual C++. Vous devez placer toutes les ressources non modifiables par 
    l'éditeur de ressources dans ce fichier.
[!endif]

res\[!output PROJECT_NAME].ico
    Il s'agit d'un fichier icône, qui est utilisé comme icône de l'application.  
Cette 
    icône est incluse par le fichier de ressource principal.

[!if HAS_SUFFIX]
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
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

Pour la fenêtre frame principale :
    Le projet comprend une interface MFC standard.

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    Ces fichiers contiennent la classe de frame [!output MAIN_FRAME_CLASS], qui 
est dérivée de CFrameWnd et contrôle toutes les fonctionnalités de frame SDI.

[!if CMD_BAR_MENUNBUTTON || CMD_BAR_BROWSERSTYLE]
res\Cmdbar.bmp
    Ce fichier bitmap sert à créer des images en mosaïque pour la barre de 
commandes.
    La barre de commandes initiale est construite dans la classe [!output 
MAIN_FRAME_CLASS]. 
    Modifiez ce fichier bitmap cmdbar avec le tableau contenu dans [!output 
MAIN_FRAME_IMPL] 
    pour ajouter d'autres boutons de barre de commandes.
[!endif]

/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
L'Assistant Application crée un type de document et une vue :

[!output DOC_HEADER], [!output DOC_IMPL] - le document
    Ces fichiers contiennent votre classe [!output DOC_CLASS].  Modifiez ces 
fichiers pour 
    ajouter les données de document spéciales et implémenter l'enregistrement et 
le 
    chargement des fichiers (via [!output DOC_CLASS]::Serialize).
[!if HAS_SUFFIX]
    Le document contiendra les chaînes suivantes :
        Extension de fichier :      [!output FILE_EXTENSION]
        ID du type de fichier :        [!output FILE_TYPE_ID]
        Titre du frame principal :  [!output MAIN_FRAME_CAPTION]
        Nom du type de document :       [!output DOC_TYPE_NAME]
        Nom de filtre :         [!output FILTER_NAME]
        Nom court de nouveau fichier : [!output FILE_NEW_NAME_SHORT]
        Nom long du type de fichier : [!output FILE_NEW_NAME_LONG]
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL] - la vue du document
    Ces fichiers contiennent votre classe [!output VIEW_CLASS].
    Les objets [!output VIEW_CLASS] servent à afficher les objets [!output 
DOC_CLASS].

[!endif]

[!else]
/////////////////////////////////////////////////////////////////////////////

L'Assistant Application crée une classe de boîte de dialogue :

[!output DIALOG_HEADER], [!output DIALOG_IMPL] – la boîte de dialogue
    Ces fichiers contiennent votre classe [!output DIALOG_CLASS].  Cette classe 
définit 
    le comportement de la boîte de dialogue principale de votre application.  Le 
modèle de boîte de dialogue se trouve 
    dans le fichier de ressources principal, qui peut être modifié dans 
Microsoft Visual C++.
[!endif]

[!if HELPSTYLE_WINHELP]
/////////////////////////////////////////////////////////////////////////////

Prise en charge de l'aide :

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

[!if ACTIVEX_CONTROLS || PRINTING || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

Autres fonctionnalités :
[!if APP_TYPE_DLG]
[!else]
[!if CMD_BAR_MENUONLY]

   Barre de commandes dans le frame principal avec menu et ornements
[!else]
[!if CMD_BAR_MENUNBUTTON]
   Barre de menus dans le frame principal avec menu, boutons et ornements
[!endif]
[!endif]
[!if STATUS_BAR]
   Barre d'état initiale dans le frame principal
[!endif]
[!endif]
[!if HELPSTYLE_WINHELP]

   Prise en charge de l'aide de Windows CE
[!endif]
[!if DYNAMIC_MFC]

   Utilise l'implémentation des DLL partagées
[!endif]
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
    nommé [!output PROJECT_NAME].pch et un fichier de types précompilés nommé 
StdAfx.obj.

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
    Il s'agit du ficher d'en-tête standard, qui définit les nouveaux ID de 
ressources.
    Microsoft Visual C++ lit et met à jour ce fichier.

/////////////////////////////////////////////////////////////////////////////

Autres remarques :

L'Assistant Application utilise "TODO:" pour indiquer les parties du code source 
où vous devrez ajouter ou modifier du code.
[!if APP_TYPE_SDI || APP_TYPE_DLG]

Si votre application utilise les MFC dans une DLL partagée et que sa langue  
n'est pas celle du système d'exploitation, vous devez copier le fichier des  
ressources localisées MFC80XXX.DLL correspondant à votre langue dans le 
répertoire de votre application  ("XXX" désigne ici l'abréviation de la langue.  
Par exemple,  
le fichier MFC80DEU.DLL contient des ressources traduites en allemand.)  Si vous 
oubliez de le faire, 
certains éléments de l'interface utilisateur de votre application resteront dans 
la langue du système d'exploitation.
[!endif]

/////////////////////////////////////////////////////////////////////////////
