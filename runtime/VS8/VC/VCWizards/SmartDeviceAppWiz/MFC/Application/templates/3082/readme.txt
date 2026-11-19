================================================================================
    BIBLIOTECA MICROSOFT FOUNDATION CLASS: Información general del proyecto[!output PROJECT_NAME]
===============================================================================

El Asistente para aplicaciones ha creado esta aplicación [!output PROJECT_NAME].  Esta aplicación no sólo muestra los fundamentos del uso de Microsoft 
Foundation Classes sino que también supone un punto de partida para el desarrollo de la propia aplicación.

Este archivo incluye un resumen acerca del contenido de los archivos que
constituyen su aplicación [!output PROJECT_NAME].

[!output PROJECT_NAME].vcproj
    Éste es el archivo de proyecto principal para los proyectos de VC++ generados mediante un asistente para aplicaciones. 
    Contiene información acerca de la versión de Visual C++ con la que se generó el archivo, así como
    información acerca de las plataformas, configuraciones y características del proyecto seleccionadas en el
    Asistente para aplicaciones.

[!output APP_HEADER]
    Éste es el archivo de encabezado principal para la aplicación.  Incluye otros encabezados
    específicos del proyecto y declara la clase de aplicación
    [!output APP_CLASS].

[!output APP_IMPL]
    Éste es el archivo fuente principal de la aplicación contenido en la clase de 
    la aplicación [!output APP_CLASS].

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
    Éste es el listado del archivo principal de recursos del proyecto de todos los
    recursos de Microsoft Windows que utiliza el proyecto.  Incluye los iconos, mapas de bits 
    y cursores almacenados en el subdirectorio RES.  Este archivo puede editarse directamente 
    en Microsoft Visual C++. Los recursos de su proyecto se encuentran en [!output LCID].
    Cuando se conserva el archivo .rc, se conservan los elementos define de la sección de datos 
    como la versión hexadecimal del valor numérico en el que están definidos en lugar del nombre descriptivo del elemento define.

res\[!output RC2_FILE_NAME].rc2
    Este archivo incluye recursos no editados por Microsoft 
    Visual C++. Debe colocar todos los recursos que no se pueden editar en 
    los editores de recursos de este archivo.
[!endif]

[!if POCKETPC2003_UI_MODEL]
[!output PROJECT_NAME]ppc.rc
    Éste es el listado del archivo de recursos principal del proyecto de todos los recursos de Microsoft Windows
    que utiliza el proyecto al compilar para la plataforma Pocket PC, o
    para una plataforma compatible con el mismo modelo de la interfaz de usuario.  Incluye los iconos,
    mapas de bits y cursores almacenados en el subdirectorio RES.  Este archivo puede 
    editarse directamente en Microsoft Visual C++. Los recursos de su proyecto 
    se encuentran en [!output LCID]. Cuando se conserva el archivo .rc, se conservan los elementos define de la sección de datos 
    como la versión hexadecimal del valor numérico en el que están definidos 
    en lugar del nombre descriptivo del elemento define.

res\[!output RC2_FILE_NAME]ppc.rc2
    Este archivo incluye recursos no editados por Microsoft 
    Visual C++. Debe colocar todos los recursos que no se pueden editar en 
    los editores de recursos de este archivo.
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
[!output PROJECT_NAME]sp.rc
    Éste es el listado del archivo principal de recursos del proyecto de todos los recursos de Microsoft Windows
    que utiliza el proyecto al compilar para la plataforma Smartphone, o
    para una plataforma compatible con el mismo modelo de la interfaz de usuario.  Incluye los iconos,
    mapas de bits y cursores almacenados en el subdirectorio RES.  Este archivo puede 
    editarse directamente en Microsoft Visual C++. Los recursos de su proyecto se encuentran en 
    [!output LCID]. Cuando se conserva el archivo .rc, se conservan los elementos define de la sección de datos 
    como la versión hexadecimal del valor numérico en el que están definidos 
    en lugar del nombre descriptivo del elemento define.

res\[!output RC2_FILE_NAME]sp.rc2
    Este archivo incluye recursos no editados por Microsoft 
    Visual C++. Debe colocar todos los recursos que no se pueden editar en 
    los editores de recursos de este archivo.
[!endif]
[!if AYGSHELL_UI_MODEL]
[!output PROJECT_NAME]ayg.rc
    Éste es el listado del archivo principal de recursos del proyecto de todos los recursos de Microsoft 
    Windows que utiliza el proyecto al compilar para una plataforma compatible con el modelo de la interfaz de usuario AYGShell.  Incluye los iconos, mapas de bits 
    y cursores almacenados en el subdirectorio RES.  Este archivo puede editarse directamente 
    en Microsoft Visual C++. Los recursos de su proyecto se encuentran en [!output LCID].
    Cuando se conserva el archivo .rc, se conservan los elementos define de la sección de datos 
    como la versión hexadecimal del valor numérico en el que están definidos en lugar del nombre descriptivo del elemento define.

res\[!output RC2_FILE_NAME]ayg.rc2
    Este archivo incluye recursos no editados por Microsoft 
    Visual C++. Debe colocar todos los recursos que no se pueden editar en 
    los editores de recursos de este archivo.
[!endif]

res\[!output PROJECT_NAME].ico
    Éste es un archivo de icono, que se utiliza como el icono de la aplicación.  Este
    icono está incluido en el archivo de recursos principal.

[!if HAS_SUFFIX]
[!output PROJECT_NAME].reg
    Éste es un ejemplo de archivo .reg que le muestra el tipo de valores de registro que establecerá el marco de trabajo.  Puede utilizarlo como un archivo .reg
[!if APP_TYPE_DLG]
    que acompañe a su aplicación.
[!else]
    que acompañe a su aplicación, o eliminarlo y basarse en el registro predeterminado RegisterShellFileTypes.
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

Para la ventana de marco principal:
    El proyecto incluye una interfaz MFC estándar.

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    Estos archivos contienen la clase de marco [!output MAIN_FRAME_CLASS], que se deriva de 
    CFrameWnd y que controla todas las características del marco SDI.

[!if CMD_BAR_MENUNBUTTON || CMD_BAR_BROWSERSTYLE]
res\Cmdbar.bmp
    Este archivo de mapa de bits se utiliza para crear imágenes organizadas en mosaico para la barra de comandos.
    La barra de comandos inicial se crea en la clase [!output MAIN_FRAME_CLASS]. 
    Modifique este mapa de bits junto con la matriz en [!output MAIN_FRAME_IMPL]
    para agregar más botones de la barra de comandos.
[!endif]

/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
El Asistente para aplicaciones crea un tipo de documento y una sola vista:

[!output DOC_HEADER], [!output DOC_IMPL] – el documento
    Estos archivos contienen su clase [!output DOC_CLASS].  Modifique estos archivos para 
    agregar los datos del documento especial y para guardar y cargar el archivo 
    (mediante [!output DOC_CLASS]::Serialize::Serialize).
[!if HAS_SUFFIX]
    El documento constará de las siguientes cadenas:
        Extensión del archivo:      [!output FILE_EXTENSION]
        Id. del tipo de archivo:        [!output FILE_TYPE_ID]
        Título del marco principal:  [!output MAIN_FRAME_CAPTION]
        Nombre del tipo de documento:       [!output DOC_TYPE_NAME]
        Nombre del filtro:         [!output FILTER_NAME]
        Nuevo nombre corto del archivo: [!output FILE_NEW_NAME_SHORT]
        Nombre largo del tipo de archivo: [!output FILE_NEW_NAME_LONG]
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL] – la vista del documento
    Estos archivos contienen su clase [!output VIEW_CLASS].
    Los objetos [!output VIEW_CLASS] se utilizan para ver objetos [!output DOC_CLASS].

[!endif]

[!else]
/////////////////////////////////////////////////////////////////////////////

El Asistente para aplicaciones crea una sola clase de cuadro de diálogo:

[!output DIALOG_HEADER], [!output DIALOG_IMPL] – el cuadro de diálogo
    Estos archivos contienen su clase [!output DIALOG_CLASS].  Esta clase define 
    el comportamiento del cuadro de diálogo principal de su aplicación.  La plantilla del cuadro de diálogo está en 
    el archivo principal de recursos, que puede editarse en Microsoft Visual C++.
[!endif]

[!if HELPSTYLE_WINHELP]
/////////////////////////////////////////////////////////////////////////////

Compatibilidad con la Ayuda:

hlp\[!output PROJECT_NAME].hpj
    Este archivo es el archivo de proyecto de ayuda que utiliza el compilador de ayudas para crear
    el archivo de ayuda de su aplicación.

hlp\*.bmp
    Éstos son archivos de mapa de bits que necesitan los temas de archivo de la Ayuda estándar para 
    los comandos estándar de la biblioteca Microsoft Foundation Class.

hlp\*.rtf
    Estos archivos contienen los temas de la Ayuda estándar para
    comandos y objetos de pantalla estándar de MFC.
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

Otras características:
[!if APP_TYPE_DLG]
[!else]
[!if CMD_BAR_MENUONLY]

   Barra de comandos en el marco principal con menús y opciones gráficas
[!else]
[!if CMD_BAR_MENUNBUTTON]
   Barra de menú en el marco principal con menú, botones y opciones gráficas
[!endif]
[!endif]
[!if STATUS_BAR]
   Barra de estado inicial en marco principal
[!endif]
[!endif]
[!if HELPSTYLE_WINHELP]

   Compatibilidad de la Ayuda en Windows CE
[!endif]
[!if DYNAMIC_MFC]

   Utiliza la implementación de archivos DLL compartidos
[!endif]
[!if ACTIVEX_CONTROLS]

Controles ActiveX
    La aplicación es compatible con el uso de los controles ActiveX.
[!endif]
[!if PRINTING]

Compatibilidad con la impresión y la vista preliminar
    Este Asistente para aplicaciones genera el código necesario para controlar los comandos de impresión, la configuración de la impresora y la vista preliminar
    mediante llamadas a funciones miembro de la clase CView de la biblioteca MFC.
[!endif]
[!if SOCKETS]

Windows Sockets
    La aplicación puede establecer comunicaciones con redes TCP/IP.
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

Otros archivos estándar:

StdAfx.h, StdAfx.cpp
    Estos archivos se utilizan para generar un archivo de encabezado precompilado (PCH) 
    denominado [!output PROJECT_NAME].pch y un archivo de tipos precompilado llamado StdAfx.obj.

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
    Éste es el archivo de encabezado estándar, que define nuevos identificadores de recurso.
    Microsoft Visual C++ lee y actualiza este archivo.

/////////////////////////////////////////////////////////////////////////////

Otras notas:

Este Asistente para aplicaciones utiliza "TODO:" para indicar las partes del código fuente que tendría que agregar o personalizar.
[!if APP_TYPE_SDI || APP_TYPE_DLG]

Si su aplicación utiliza MFC en un archivo DLL compartido, y el idioma de su aplicación es distinto al idioma actual del sistema operativo, tendrá que copiar los recursos localizados correspondientes MFC80XXX.DLL en el directorio de su aplicación ("XXX" son las iniciales de la abreviatura del idioma.  Por ejemplo, 
MFC80DEU.DLL contiene recursos traducidos al alemán.)  Si no lo hace, 
algunos de los elementos de la interfaz de usuario de su aplicación se mantendrán en el idioma del sistema operativo.
[!endif]

/////////////////////////////////////////////////////////////////////////////
