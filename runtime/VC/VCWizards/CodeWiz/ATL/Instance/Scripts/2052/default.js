// Script for WMI Instance provider wizard

function OnFinish(selProj, selObj)
{

    var oCM = selProj.CodeModel;
    try
    {   
    
        oCM.StartTransaction("Add WMI Instance provider");
    
                
        var bDLL;
        if (typeDynamicLibrary == selProj.Object.Configurations(1).ConfigurationType) //REVIEW: hardcoded configuration
            bDLL = true;
        else
            bDLL = false;
                
        wizard.AddSymbol("DLL_APP", bDLL);
        
        var strProjectName      = wizard.FindSymbol("PROJECT_NAME");
        var strProjectPath      = wizard.FindSymbol("PROJECT_PATH");
        var strTemplatePath     = wizard.FindSymbol("TEMPLATES_PATH");
        var strShortName        = wizard.FindSymbol("SHORT_NAME");
        var strUpperShortName   = CreateASCIIName(strShortName.toUpperCase());
        wizard.AddSymbol("UPPER_SHORT_NAME", strUpperShortName);
                
        var strVIProgID         = wizard.FindSymbol("VERSION_INDEPENDENT_PROGID");
        wizard.AddSymbol("PROGID",  strVIProgID.substr(0,37) + ".1");
        var strClassName        = wizard.FindSymbol("CLASS_NAME");
        var strHeaderFile       = wizard.FindSymbol("HEADER_FILE");
        var strImplFile         = wizard.FindSymbol("IMPL_FILE");
        var strCoClass          = wizard.FindSymbol("COCLASS");
        var bAttributed = wizard.FindSymbol("ATTRIBUTED");
        
        var strProjectCPP       = GetProjectFile(selProj, "CPP", false, false);
        var strProjectRC        = GetProjectFile(selProj, "RC", true, false);
        
        var bClassSpecified = (wizard.FindSymbol("WMICLASSNAME").toString() != "");
        wizard.AddSymbol ("CLASS_SPECIFIED", bClassSpecified);
        if (bClassSpecified)
        {
            //add "dynamic" and "provider" qualifiers, if necessary
            
            var locator = new ActiveXObject("WbemScripting.SWbemLocator");
            var services = locator.ConnectServer("", //local server
                            wizard.FindSymbol("NAMESPACE"));                                                
            var classObj = services.Get(wizard.FindSymbol("WMICLASSNAME"));
            
            AddQualifier (classObj, 
                        "provider", 
                        wizard.FindSymbol("SHORT_NAME"),
                        false,
                        true,
                        true);
                            
            AddQualifier (classObj, 
                        "dynamic", 
                        true,
                        false,
                        true,
                        true);
                                                                                            
        }
        
        // Create necessary GUIDS
        CreateGUIDs();
                
        if (!bAttributed)
        {
            // Get LibName
            wizard.AddSymbol("LIB_NAME", oCM.IDLLibraries(1).Name);
    
            // Get LibID
            wizard.AddSymbol("LIBID_REGISTRY_FORMAT", oCM.IDLLibraries(1).Attributes("uuid").Value);
        
            // Get AppID
            var strAppID = wizard.GetAppID();
            if (strAppID.length > 0)
            {
                wizard.AddSymbol("APPID_EXIST", true);
                wizard.AddSymbol("APPID_REGISTRY_FORMAT", strAppID);
            }
            
            // add RGS file resource
            var strRGSFile = GetUniqueFileName(strProjectPath, CreateASCIIName(strShortName) + ".rgs");
            var strRGSID = "IDR_" + strUpperShortName;
            RenderAddTemplate(wizard, "wmiprov.rgs", strRGSFile, false);
    
            var oResHelper = wizard.ResourceHelper;

            oResHelper.OpenResourceFile(strProjectRC);
            //oResHelper.AddResource(strRGSID, strProjectPath + "\\" + strRGSFile, "REGISTRY");
            oResHelper.AddResource(strRGSID, strProjectPath + strRGSFile, "REGISTRY");
            oResHelper.CloseResourceFile(); 
                                                
            // Render wmiprovco.idl and insert into strProject.idl
            AddCoclassFromFile(oCM, "wmiprovco.idl");       
                        
            SetMergeProxySymbol(selProj);
        
        }

        // Call this function to set the QuerySupportLevelProperty
        SetQuerySupportLevels();
        
        // Add #include "strHeaderFile" to strProjectName.cpp
        if (!DoesIncludeExist(selProj, '"' + strHeaderFile + '"', strProjectCPP))
            oCM.AddInclude('"' + strHeaderFile + '"', strProjectCPP, vsCMAddPositionEnd);


        // Add header
        RenderAddTemplate(wizard, "wmiprov.h", strHeaderFile, selObj);
        
        // Add CPP
        RenderAddTemplate(wizard, "wmiprov.cpp", strImplFile, selObj);
    
        //Add MOF
        AddMOFFromFile("wmiprov.mof", selProj);
        
        //add wmi libraries to linker input
        var nTotal = selProj.Object.Configurations.Count;
        var nCntr;      
        for (nCntr = 1; nCntr <= nTotal; nCntr++)
        {
                    
            var VCLinkTool = selProj.Object.Configurations(nCntr).Tools("VCLinkerTool");
            if (-1 == VCLinkTool.AdditionalDependencies.toUpperCase().search("WBEMUUID.LIB"))
            {
                VCLinkTool.AdditionalDependencies += " wbemuuid.lib";
            }
            if (-1 == VCLinkTool.AdditionalDependencies.toUpperCase().search("WMIUTILS.LIB"))
            {
                VCLinkTool.AdditionalDependencies += " wmiutils.lib";
            }
        }
        
        oCM.CommitTransaction();
                    
    }
    catch(e)
    {
        oCM.AbortTransaction();
        var L_alert1_ErrorMessage = "OnFinish 中出错: ";
        wizard.ReportError( L_alert1_ErrorMessage + e.description);
    }
    return;
}// end function OnFinsh


function AddMOFFromFile (strTemplateName, selProj)
{
    // if project.mof doesn't exist in the project already create one
    // append the mof for the new class at the end of the new or existing mof

    try
    {
        var ForReading = 1, ForWriting = 2, ForAppending = 8;
            var TristateUseDefault = -2, TristateUnicode = -1, TristateAnsi = 0;
    
        var fso = new ActiveXObject("Scripting.FileSystemObject");
        var strMOFName = wizard.FindSymbol("PROJECT_NAME") + ".mof";
        
        var strMOFPath = wizard.FindSymbol("PROJECT_PATH") + strMOFName;        
        
        var bFirstRun = !fso.FileExists(strMOFPath);


        if (bFirstRun)
        {
            // create the initial mof file with header and add it to sources "folder" in project
        
            var folder;         
            //add MOF filter to "Source Files"                  
            for( var n = 1 ; n <= selProj.Object.Filters.Count ; n++)
            {
                folder = selProj.Object.Filters.Item(n);
                if( folder.Filter.match( "cpp" ) != null  )
                {
                    folder.Filter = folder.Filter + ";mof"
                    break;
                }
            }
                        
            RenderAddTemplate(wizard, "header.mof", strMOFName, false);

            // Close the opened .mof file, so that there is no warning message
            // for opened modified file
            CloseProjFile(strMOFName,selProj);
        }
        
        // now the header is in the strMOFName file within the project
        // or from the past the header is there with one or more classes
        // build the information for new mof for the new class and append

        
        if (wizard.FindSymbol("CLASS_SPECIFIED") == true)
        {
            var strEscapedNS = BuildEscapedNS();    
            wizard.AddSymbol("ESCAPED_NAMESPACE", strEscapedNS);    
        }
        
        RenderAddTemplate(wizard, strTemplateName, strMOFName, false);    
        
        if(bFirstRun)
        {      
            AddFileToProject(strMOFName,selProj.ProjectItems ,true)                 

            //set custom build step for the MOF file in each configuration
            
            var files = selProj.Object.Files;
            var Moffile = files(strMOFName);

            var nTotal = Moffile.FileConfigurations.Count;          
            var nCntr;      
            for (nCntr = 1; nCntr <= nTotal; nCntr++)
            {               
                var customBuildTool = Moffile.FileConfigurations(nCntr).Tool;
                                
                customBuildTool.CommandLine = "mofcomp \"$(InputDir)\\$(InputName)\".mof\n\recho mofcomptrace > \"$(OutDir)\\$(InputName)\".trace";
                
                customBuildTool.Outputs  =  "\"$(OutDir)\\$(InputName)\".trace";    
                
                var L_alert10_ErrorMessage = "正在编译 MOF 文件";
                customBuildTool.Description = L_alert10_ErrorMessage;                                   
            }           
        }                               
        // To Close the opened .mof file, so that there is no warning message
        // for opened modified file
        CloseProjFile(strMOFName, selProj);

    }
    catch(e)
    {
        var L_alert2_ErrorMessage = "AddMOFFile 中出错: ";
        wizard.ReportError(L_alert2_ErrorMessage + e.description);
    }
    return;
}// end function AddMOFFromFile

// helpers

function ProjFileOpen(strFileName, selProj)
{
    // strFileName does not contain path - just simple name
    var bFileOpened = false;
    // Check if MOF file is already opened
    var activeDoc;
    var nDocs;
    for(nDocs = 1 ; nDocs <= selProj.DTE.Documents.Count ; nDocs++)
    {
        activeDoc =selProj.DTE.Documents.Item(nDocs);
        if(activeDoc.Name == strFileName)
        {
            bFileOpened = true;
            break;
        }
    }
    return bFileOpened;
}

function CloseProjFile(strFileName, selProj)
{
    // close an open project file by name
    // strFileName does not contain path - just simple name
    var activeDoc;
    var nDocs;
    for(nDocs = 1 ; nDocs <= selProj.DTE.Documents.Count ; nDocs++)
    {
        activeDoc =selProj.DTE.Documents.Item(nDocs);
        if(activeDoc.Name == strFileName)
        {
            activeDoc.Close();
            break;
        }
    }
    return;
}

function BuildEscapedNS()
{
    //create ESCAPED_NAMESPACE symbol that would have 2 backslashes in
    //complex namespace names

    var strEscapedNS = "";
    var strNS = wizard.FindSymbol("NAMESPACE");
    var arPieces = strNS.split("\\");               

    if (arPieces.length == 0)
    {
        strEscapedNS = strNS;
    }
    else 
    {           
        for (var i = 0; i < arPieces.length - 1; i++)
        {
            strEscapedNS += arPieces[i] + "\\\\";
        }                                       
        strEscapedNS += arPieces[arPieces.length - 1];          
    }
    return strEscapedNS;
}

function CreateGUIDs()
{
    try
    {
        // create CLSID
        var strRawGUID = wizard.CreateGuid();
        var strFormattedGUID = wizard.FormatGuid(strRawGUID, 0);
        wizard.AddSymbol("CLSID_REGISTRY_FORMAT", strFormattedGUID);

        
    }
    catch(e)
    {
        var L_alert3_ErrorMessage = "CreateGUIDs() 中出错: ";
        wizard.ReportError(L_alert3_ErrorMessage + e.description);
    }
}

function AddQualifier (classObj, 
                            qualName, 
                            qualValue,
                            bToSubClasses,
                            bToInstances,
                            bOverridable)
{
    try 
    {   
        var theQual;
        try
        {
            theQual = classObj.Qualifiers_(qualName);
            theQual.Value = qualValue;
        }
        catch (e)
        {
            //if (e.number == wbemErrNotFound)  //qualifier not found
            //{
            if (e.number == -2147217406)  //qualifier not found
            {
                theQual = classObj.Qualifiers_.Add(qualName, 
                                                    qualValue, 
                                                    bToSubClasses,
                                                    bToInstances,
                                                    bOverridable);
            }
            else
            {
                throw(e);   //re-throw
            }
        }
        
        //commit changes to WMI
        classObj.Put_();
    }
    catch (e)
    {
        var L_alert4_ErrorMessage = "AddQualifier() 中出错: ";
        wizard.ReportError (L_alert4_ErrorMessage + e.description);
    }

}


function IsWMIOk (selProj, selObj)
{
    //uses WMI Registry provider to determine WMI version
        
    try 
    {
        if (!CanAddATLClass(selProj, selObj))
        {
            return false;
        }
        try 
        {
            var locator = new ActiveXObject("WbemScripting.SWbemLocator");
        
            var services = locator.ConnectServer("", //local server
                                        "root\\default");                                               
        }
        catch (e)
        {
            var L_alert5_ErrorMessage = "无法连接到 WMI。请验证计算机上已安装 WMI";
            throw(L_alert5_ErrorMessage);
        }
                
        var classObj = services.Get("StdRegProv");
                
        var methObject = classObj.Methods_.Item("GetExpandedStringValue");
        var inParmsInstance = methObject.InParameters.SpawnInstance_();
                
        inParmsInstance.Properties_.Add("sSubKeyName", 8);
    
        inParmsInstance.Properties_("sSubKeyName").Value = 
            "SOFTWARE\\Microsoft\\WBEM";
    

        inParmsInstance.Properties_.Add("sValueName", 8);
    
        inParmsInstance.Properties_("sValueName").Value = "Build";


        var outParms = classObj.ExecMethod_("GetExpandedStringValue",
                                            inParmsInstance);
                                            
        if (outParms.Properties_("ReturnValue").Value != 0)
        {
            var L_alert6_ErrorMessage = "无法确定 WMI 内部版本号";
            throw (L_alert6_ErrorMessage);
        }

        if (outParms.Properties_("sValue").Value >= "1085")
        {
            return true;
        }
        else 
        {   
            var L_alert7_ErrorMessage = "您的计算机上安装的 WMI 的版本是 ";
            var L_alert8_ErrorMessage = "。要求有版本 1085 或更高版本，才能运行此向导。";
            throw (L_alert7_ErrorMessage + outParms.Properties_("sValue").Value +
                    L_alert8_ErrorMessage);
        }
    }

    catch (e)
    {
        var L_alert9_ErrorMessage = "由于以下错误，无法将 WMI 实例提供程序对象添加到项目中: \n\r";
        wizard.ReportError (L_alert9_ErrorMessage + e);
        return false;
    }

    
}

function SetQuerySupportLevels()
{
    var strQuerySupportLevels="";
    var bIsFirst = true;
    
    if(wizard.FindSymbol("QUERY_ASSOCIATORS").toString().toUpperCase() == "TRUE")
    {
        strQuerySupportLevels += "\"WQL:Associators\"";
        bIsFirst = false;
    }

    if(wizard.FindSymbol("QUERY_CUSTOM").toString().toUpperCase() == "TRUE")
    {
        if(bIsFirst == false)
        {
            strQuerySupportLevels += ",";
        }
        strQuerySupportLevels += "\"WQL:V1ProviderDefined\"";
        bIsFirst = false;
    }

    if(wizard.FindSymbol("QUERY_UNARY").toString().toUpperCase() == "TRUE")
    {
        if(bIsFirst == false)
        {
            strQuerySupportLevels += ",";
        }
        strQuerySupportLevels += "\"WQL:UnarySelect\"";
        bIsFirst = false;
    }

    if(wizard.FindSymbol("QUERY_REFERENCES").toString().toUpperCase() == "TRUE")
    {
        if(bIsFirst == false)
        {
            strQuerySupportLevels += ",";
        }
        strQuerySupportLevels += "\"WQL:References\"";
        bIsFirst = false;
    }
    
    if(bIsFirst == false)
    {
        wizard.AddSymbol("QUERYSUPPORTS_LEVEL",strQuerySupportLevels);
        wizard.AddSymbol("SUPPORTS_QUERY",true);
    }

    
}

