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

[!output PROJECT_NAME].rc
    Ésta es una lista de todos los recursos de Microsoft Windows que utiliza el 
    programa.  Incluye los iconos, mapas de bits y cursores almacenados en el 
    subdirectorio RES.  Este archivo puede editarse directamente en Microsoft
    Visual C++. Los recursos de su proyecto se encuentran en [!output LCID].

res\[!output PROJECT_NAME].ico
    Éste es un archivo de icono, que se utiliza como el icono de la aplicación.     Éste
    icono está incluido en el archivo principal de recursos [!output PROJECT_NAME].rc.

res\[!output RC2_FILE_NAME].rc2
    Este archivo incluye recursos no editados por Microsoft 
    Visual C++. Debe colocar todos los recursos que no se pueden editar en 
    los editores de recursos de este archivo.
[!if CONTAINER_SERVER || FULL_SERVER || MINI_SERVER || AUTOMATION || HAS_SUFFIX]
[!if !HTML_EDITVIEW]

[!output PROJECT_NAME].reg
    Éste es un ejemplo de archivo .reg que le muestra el tipo de valores de registro que establecerá el marco de trabajo.  Puede utilizarlo como un archivo .reg
[!if APP_TYPE_DLG]
    que acompañe a su aplicación.
[!else]
    que acompañe a su aplicación, o eliminarlo y basarse en el registro predeterminado RegisterShellFileTypes.
[!endif]

[!if AUTOMATION]
[!output SAFE_IDL_NAME].idl
    Este archivo contiene el código fuente del Lenguaje de descripción de interfaces para la
    biblioteca de tipos de su aplicación.
[!endif]
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

Para la ventana de marco principal:
[!if PROJECT_STYLE_EXPLORER]
    Estilo del Explorador de Windows: el proyecto incluirá una interfaz similar a la del Explorador de Windows, 
    con dos marcos.
[!else]
    El proyecto incluye una interfaz MFC estándar.
[!endif]

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    Estos archivos contienen la clase de marco [!output MAIN_FRAME_CLASS], que se deriva de
[!if APP_TYPE_MDI]
    CMDIFrameWnd y que controla todas las características del marco MDI.
[!else]
    CFrameWnd y que controla todas las características del marco SDI.
[!endif]
[!if PROJECT_STYLE_EXPLORER]

[!output TREE_VIEW_HEADER], [!output TREE_VIEW_IMPL]
    Estos archivos contienen la clase de marco de la parte izquierda [!output TREE_VIEW_CLASS], que se deriva de CTreeView.
[!endif]

[!if DOCKING_TOOLBAR]
res\Toolbar.bmp
    Este archivo de mapa de bits se utiliza para crear imágenes organizadas en mosaico para la barra de herramientas.
    La barra de herramientas inicial y la barra de estado se crean en la clase [!output MAIN_FRAME_CLASS]. Edite este mapa de bits de barra de herramientas utilizando los editores de recursos, y
    actualice la matriz IDR_MAINFRAME TOOLBAR en [!output PROJECT_NAME].rc para agregar botones de barra de herramientas.
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

res\IToolbar.bmp
    Este archivo de mapa de bits se utiliza para crear imágenes organizadas en mosaico para la barra de herramientas, cuando la
    la aplicación de servidor se activa en el contexto dentro de otra aplicación contenedora. Esta barra de herramientas se crea en la clase [!output INPLACE_FRAME_CLASS]. Este mapa de bits es similar al mapa de bits de res\Toolbar.bmp
    salvo que se le han quitado muchos comandos no relacionados con el servidor.

[!endif]
[!endif]
[!if APP_TYPE_MDI]
/////////////////////////////////////////////////////////////////////////////

Para la ventana de marco secundaria:

ChildFrm.h, ChildFrm.cpp
    Estos archivos definen e implementan la clase [!output CHILD_FRAME_CLASS], que permite ventanas secundarias en una aplicación MDI.

[!endif]
/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
[!if !DB_VIEW_NO_FILE]
El Asistente para aplicaciones crea un tipo de documento y una sola vista:

[!output DOC_HEADER], [!output DOC_IMPL] – el documento
    Estos archivos contienen su clase [!output DOC_CLASS].  Edite estos archivos para 
    agregar los datos del documento especial y para guardar y cargar el archivo 
    (mediante [!output DOC_CLASS]::Serialize).
[!if HAS_SUFFIX && !HTML_EDITVIEW]
    El documento constará de las siguientes cadenas:
        Extensión del archivo:      [!output FILE_EXTENSION]
        Id. del tipo de archivo:        [!output FILE_TYPE_ID]
        Título del marco principal:  [!output MAIN_FRAME_CAPTION]
        Nombre del tipo de documento:       [!output DOC_TYPE_NAME]
        Nombre del filtro:         [!output FILTER_NAME]
        Nuevo nombre corto del archivo: [!output FILE_NEW_NAME_SHORT]
        Nombre largo del tipo de archivo: [!output FILE_NEW_NAME_LONG]
[!endif]
[!else]
El Asistente para aplicaciones crea una sola vista:
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL] – la vista del documento
    Estos archivos contienen su clase [!output VIEW_CLASS].
[!if !DB_VIEW_NO_FILE]
    Los objetos [!output VIEW_CLASS] se utilizan para ver objetos [!output DOC_CLASS].
[!endif]

[!if APP_TYPE_MDI]
res\[!output PROJECT_NAME]Doc.ico
    Éste es un archivo de icono, que se utiliza como el icono para las ventanas secundarias MDI 
    para la clase [!output DOC_CLASS].  Este icono lo incluye el archivo principal
    de recursos [!output PROJECT_NAME].rc.
[!endif]

[!endif]

[!if DB_VIEW_NO_FILE || DB_VIEW_WITH_FILE]
/////////////////////////////////////////////////////////////////////////////

Compatibilidad con bases de datos:

[!output ROWSET_HEADER], [!output ROWSET_IMPL]
    Estos archivos contienen su clase [!output ROWSET_CLASS]. Esta clase se utiliza para obtener acceso a la fuente de datos 
    que seleccionó en el Asistente.
[!if DB_VIEW_NO_FILE]
    No se agregará compatibilidad de serialización.
[!else]
    Se ha agregado compatibilidad de serialización.
[!endif]
[!endif]
[!if CONTAINER || FULL_SERVER || MINI_SERVER || CONTAINER_SERVER]
/////////////////////////////////////////////////////////////////////////////

El Asistente para aplicaciones también ha creado clases específicas para OLE

[!if CONTAINER || CONTAINER_SERVER]
[!output CONTAINER_ITEM_HEADER], [!output CONTAINER_ITEM_IMPL]
    Estos archivos contienen su clase [!output CONTAINER_ITEM_CLASS]. Esta clase se utiliza para
    manipular objetos OLE.  Los objetos OLE se suelen mostrar por su clase 
    [!output VIEW_CLASS] y serializarse como parte de su clase [!output DOC_CLASS].
[!if ACTIVE_DOC_CONTAINER]
    El programa es compatible con los documentos activos en su marco.
[!endif]
[!endif]
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

[!output SERVER_ITEM_HEADER], [!output SERVER_ITEM_IMPL]
    Estos archivos contienen [!output SERVER_ITEM_CLASS]. Esta clase se utiliza para
    conectar su clase [!output DOC_CLASS] con el sistema OLE y, opcionalmente, 
    proporcionar vínculos a su documento.
[!if ACTIVE_DOC_SERVER]
    El proyecto permite crear y administrar documentos activos.
[!endif]

[!output INPLACE_FRAME_HEADER], [!output INPLACE_FRAME_IMPL]
    Estos archivos contienen [!output INPLACE_FRAME_CLASS]. Esta clase se deriva 
    de COleIPFrameWnd y controla todas las características del marco durante la activación en contexto.
[!endif]

[!if SUPPORT_COMPOUND_FILES]
    El proyecto es compatible con archivos compuestos. El formato de archivos compuestos almacena un documento 
    que contiene uno o varios objetos de automatización para un archivo y sigue permitiendo el acceso al 
    mismo a objetos individuales.
[!endif]
[!endif]
[!else]

/////////////////////////////////////////////////////////////////////////////

[!if AUTOMATION]
El Asistente para aplicaciones crea una clase de cuadro de diálogo y una clase de proxy de automatización:
[!else]
El Asistente para aplicaciones crea una sola clase de cuadro de diálogo:
[!endif]

[!output DIALOG_HEADER], [!output DIALOG_IMPL] – el cuadro de diálogo
    Estos archivos contienen su clase [!output DIALOG_CLASS].  Esta clase define 
    el comportamiento del cuadro de diálogo principal de su aplicación.  La plantilla del cuadro de diálogo está en 
    [!output PROJECT_NAME].rc, que puede editarse en Microsoft Visual C++.
[!if AUTOMATION]

[!output DIALOG_AUTO_PROXY_HEADER], [!output DIALOG_AUTO_PROXY_IMPL] – el objeto de automatización
    Estos archivos contienen su clase [!output DIALOG_AUTO_PROXY_CLASS].  Esta clase
    se denomina clase de proxy de automatización para su cuadro de diálogo,
    porque se ocupa de exponer los métodos y las propiedades de automatización 
    que pueden utilizar los controladores de automatización para permitir el acceso a su cuadro de diálogo.  Estos métodos
    y propiedades no se exponen desde la clase de cuadro de diálogo directamente, porque
    en el caso de una aplicación MFC basada en un cuadro de diálogo modal es más claro y fácil 
    mantener el objeto de automatización separado de la interfaz de usuario.
[!endif]
[!endif]

[!if CONTEXT_HELP]
/////////////////////////////////////////////////////////////////////////////

Compatibilidad con la Ayuda:

[!if HELPSTYLE_HTML]
hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhp
    Este archivo es un archivo de proyecto de ayuda. Contiene los datos necesarios para compilar los archivos de ayuda en un archivo .chm.

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhc
    Este archivo muestra una lista del contenido del proyecto de ayuda.

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhk
    Este archivo contiene un índice de los temas de la ayuda.

hlp\afxcore.htm
    Este archivo contiene los temas de la ayuda estándar para comandos y objetos de pantalla estándar de MFC. Agregue sus propios temas de ayuda a este archivo.

[!if PRINTING]
hlp\afxprint.htm
    Este archivo contiene los temas de ayuda para los comandos de impresión.

[!endif]
makehtmlhelp.bat
    El sistema de generación utiliza este archivo para compilar los archivos de ayuda.

hlp\Images\*.gif
    Éstos son archivos de mapa de bits que necesitan los temas de archivo de la ayuda estándar para los comandos estándar de la biblioteca
    Microsoft Foundation Class.

[!else]
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
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SPLITTER || MAPI || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

Otras características:
[!if ACTIVEX_CONTROLS]

Controles ActiveX
    La aplicación es compatible con el uso de los controles ActiveX.
[!endif]
[!if PRINTING]

Compatibilidad con la impresión y la vista preliminar
    Este Asistente para aplicaciones genera el código necesario para controlar los comandos de impresión, la configuración de la impresora y la vista preliminar
    mediante llamadas a funciones miembro de la clase CView de la biblioteca MFC.
[!endif]
[!if DB_SUPPORT_HEADER_ONLY && !APP_TYPE_DLG]

Compatibilidad con bases de datos
    El Asistente para aplicaciones ha agregado el nivel básico de compatibilidad con bases de datos para su programa. 
    Sólo se han incluido los archivos necesarios.
[!endif]
[!if SPLITTER && !APP_TYPE_DLG]

Ventana divisora
    El Asistente para aplicaciones ha agregado compatibilidad con ventanas divisoras para los documentos de su aplicación.
[!endif]
[!if MAPI]

Compatibilidad con MAPI
    El proyecto generado contiene el código necesario para crear, manipular, transferir y
    almacenar mensajes de correo.
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

Resource.h
    Éste es el archivo de encabezado estándar, que define nuevos identificadores de recurso.
    Microsoft Visual C++ lee y actualiza este archivo.

[!if MANIFEST]
[!output PROJECT_NAME].manifest
	Windows XP utiliza los archivos de manifiesto de la aplicación para describir la dependencia de una aplicación en versiones específicas de ensamblados simultáneos. El cargador utiliza esta información para cargar el ensamblado adecuado desde la caché de ensamblados o desde el directorio privado de la aplicación. El manifiesto de la aplicación puede incluirse para su redistribución como un archivo .manifest instalado en la misma carpeta que el ejecutable de la aplicación o puede incluirse en el ejecutable en forma de recurso. 
[!endif]
/////////////////////////////////////////////////////////////////////////////

Otras notas:

El Asistente para aplicaciones utiliza "TODO:" para indicar partes del código fuente que debería agregar o personalizar.
[!if APP_TYPE_MDI || APP_TYPE_SDI || APP_TYPE_DLG || APP_TYPE_MTLD]

Si su aplicación utiliza MFC en un archivo DLL compartido, tendrá que copiar
los archivos DLL MFC. Si el idioma de su aplicación es distinto al idioma actual
del sistema operativo, también tendrá que copiar los recursos localizados correspondientes
MFC80XXX.DLL. Para obtener mas información sobre ambos temas, consulte la sección que trata
sobre al copia de aplicaciones de Visual C++ en la documentación de MSDN.
[!endif]

/////////////////////////////////////////////////////////////////////////////
