//
// 版权所有 (C) Microsoft Corporation 2001-2002。保留所有权利。
//

var vsViewKindPrimary           = "{00000000-0000-0000-0000-000000000000}";
var vsViewKindDebugging         = "{7651A700-06E5-11D1-8EBD-00A0C90F26EA}";
var vsViewKindCode              = "{7651A701-06E5-11D1-8EBD-00A0C90F26EA}";
var vsViewKindDesigner          = "{7651A702-06E5-11D1-8EBD-00A0C90F26EA}";
var vsViewKindTextView          = "{7651A703-06E5-11D1-8EBD-00A0C90F26EA}";
var vsWindowKindSolutionExplorer= "{3AE79031-E1BC-11D0-8F78-00A0C9110057}";

var GUID_ItemType_PhysicalFolder= "{6BB5F8EF-4483-11D3-8BCF-00C04F8EC28C}";
var GUID_ItemType_VirtualFolder = "{6BB5F8F0-4483-11D3-8BCF-00C04F8EC28C}";
var GUID_ItemType_PhysicalFile  = "{6BB5F8EE-4483-11D3-8BCF-00C04F8EC28C}";

var vsWizardAddSubProject       = "{0F90E1D2-4999-11D1-B6D1-00A0C90F2744}";


function CreateVSProject(strProjectName, strProjectExt, strProjectPath, strTemplateFile)
{
    var solution = dte.Solution;
    var strSolutionName = "";

    if (wizard.FindSymbol("CLOSE_SOLUTION"))
    {
        solution.Close();
        strSolutionName = wizard.FindSymbol("VS_SOLUTION_NAME");
        if (strSolutionName.length)
        {
            var strSolutionPath = strProjectPath.substr(0, strProjectPath.length - strProjectName.length);
            solution.Create(strSolutionPath, strSolutionName);
        }
    }

    var strProjectFile = strProjectName + strProjectExt;

    var oTarget = wizard.FindSymbol("TARGET");
    var project;

    if (wizard.FindSymbol("WIZARD_TYPE") == vsWizardAddSubProject)
    {
        var prjItem = oTarget.AddFromTemplate(strTemplateFile, strProjectPath+"\\"+strProjectFile);
        project = prjItem.SubProject;
    }
    else
    {
        project = oTarget.AddFromTemplate(strTemplateFile, strProjectPath, strProjectFile);
    }
    return project;
}

function AddFileToVSProject(strItemName, selProj, selObj, strTemplateFile, bValidate)
{
    fso = new ActiveXObject("Scripting.FileSystemObject");
    AddBaseNameToWizard("SAFE_ITEM_NAME", strItemName, ".");

    if( bValidate )
    {
        var strSafeName = wizard.FindSymbol( "SAFE_ITEM_NAME" );
        if( !wizard.ValidateCLRIdentifier( strSafeName, false ))
        {
            strSafeName = "_" + strSafeName;
            wizard.AddSymbol("SAFE_ITEM_NAME", strSafeName);
        }
    }

    var isReferenceExpanded;
    if(selProj != null) 
        isReferenceExpanded = IsReferenceExpanded(selProj);

    // 获取 selObj 集合的项目项
    var folder = selObj.parent;

    var strURL = folder.Properties("URL").Value;
    if (strURL.length > 0) //如果 Web 项目
        var strProjectPath = folder.Properties("LocalPath");
    else
        var strProjectPath = folder.Properties("FullPath");

    var strItemFile = strProjectPath + strItemName;

    var fsoTemporaryFolder = 2;
    var tFolder = fso.GetSpecialFolder(fsoTemporaryFolder);
    var strTempName = fso.GetTempName();
    var strTempFile = tFolder.Path + "\\" + strTempName;
    SafeDeleteFile(fso, strTempFile);
    
    wizard.RenderTemplate(strTemplateFile, strTempFile, false);
    var item = folder.ProjectItems.AddFromTemplate(strTempFile, strItemName );
    SafeDeleteFile(fso, strTempFile);

    if(selProj != null)
    {
        if(isReferenceExpanded == true)
 	    ExpandReferencesNode(selProj);
        else
            CollapseReferencesNode(selProj);
    }

    return item;
}

function AddDependentFileToVSProject(strItemName, projectItem, strTemplateFilePath)
{
    fso = new ActiveXObject("Scripting.FileSystemObject");
    
    var fsoTemporaryFolder = 2;
    var tFolder = fso.GetSpecialFolder(fsoTemporaryFolder);
    var strTempName = fso.GetTempName();
    var strTempFile = tFolder.Path + "\\" + strTempName;
    SafeDeleteFile(fso, strTempFile);
    
    wizard.RenderTemplate(strTemplateFilePath, strTempFile, false);
    var item = projectItem.ProjectItems.AddFromTemplate(strTempFile, strItemName);
    SafeDeleteFile(fso, strTempFile);
    
    return item;
}

function AddBaseNameToWizard( strName, strValue, strDelim )
{
    var strLegalItemName;
    var nIndex = strValue.lastIndexOf(strDelim);
    if( nIndex > 0 )
        strLegalItemName = strValue.substr(0, nIndex);
    else
        strLegalItemName = strValue;
    wizard.AddSymbol(strName, CreateLegalIdentifier(strLegalItemName));
}

function SafeDeleteFile( fso, strFilespec )
{
	if (fso.FileExists(strFilespec))
	{
		var tmpFile = fso.GetFile(strFilespec);
		tmpFile.Delete();
	}
}

function GetDependentFileName(strMainFileName, strMainExtension, strDependentExtension)
{
    var strDependentFileName = strMainFileName;
    
    // 如果主文件名为 Something.vb，则依赖文件名将为 Something.resx
    // 否则，如果主文件名为 Something.ext，则依赖文件名将为 Something.ext.resx。
    
    if (strMainFileName.toLowerCase().lastIndexOf(strMainExtension.toLowerCase()) == 
        strMainFileName.length - strMainExtension.length)
    {
        strDependentFileName = strMainFileName.substring(0, strMainFileName.length - strMainExtension.length) +
            strDependentExtension;
    }
    else 
    {
        strDependentFileName = strMainFileName + strDependentExtension;
    }
    
    return strDependentFileName;
}

function CreateLegalIdentifier(strName)
{
    var nLen = strName.length;
    var strLegalName = "";
    var cChar = strName.charAt(0);
    switch(cChar)
    {
        case "0":
        case "1":
        case "2":
        case "3":
        case "4":
        case "5":
        case "6":
        case "7":
        case "8":
        case "9":
            strLegalName += "_";
            break;
    }
    for (nCntr = 0; nCntr < nLen; nCntr++)
    {
        cChar = strName.charAt(nCntr);
        switch(cChar)
        {
            case " ":
            case "~":
            case "&":
            case "'":
            case "#":
            case "!":
            case "@":
            case "$":
            case "%":
            case "^":
            case "(":
            case ")":
            case "-":
            case "+":
            case "=":
            case "{":
            case "}":
            case "[":
            case "]":
            case ";":
            case ",":
            case "`":
            case ".":
                strLegalName += "_";
                break;
            default:
                strLegalName += cChar;
                break;
        }
    }
    return strLegalName;
}

function ReplaceDots(strName)
{
    var nLen = strName.length;
    var strLegalName = "";
    for (nCntr = 0; nCntr < nLen; nCntr++)
    {
        var cChar = strName.charAt(nCntr);
        if (cChar == ".")
                strLegalName += "_";
        else
                strLegalName += cChar;
    }
    return strLegalName;
}


function AddNamespaceSymbolToWizard(dtex, wizardx, selObj) 
{
    var parent = selObj.Parent;
    var kind = parent.Kind;
    if(kind == GUID_ItemType_PhysicalFolder)
    {
        wizardx.AddSymbol("NAMESPACE", parent.Properties("DefaultNamespace").Value);
    }
    else
    {
        wizardx.AddSymbol("NAMESPACE", parent.Properties("RootNamespace").Value);
    }
}


function AddDefaultClientScriptToWizard(dtex, wizardx, selProj)
{
    var prjScriptLang = selProj.Properties("DefaultClientScript").Value;
    // 0 = JScript
    // 1 = VBScript
    if(prjScriptLang == 0)
    {
        wizardx.AddSymbol("DEFAULT_CLIENT_SCRIPT", "JavaScript");
    }
    else
    {
        wizardx.AddSymbol("DEFAULT_CLIENT_SCRIPT", "VBScript");
    }
}

function AddDefaultTargetSchemaToWizard(dtex, wizardx, selProj)
{
    var prjTargetSchema = selProj.Properties("DefaultTargetSchema").Value;
    // 0 = IE3/Nav4
    // 1 = IE5
    // 2 = Nav4
    if(prjTargetSchema == 0)
    {
        wizardx.AddSymbol("DEFAULT_TARGET_SCHEMA", "http://schemas.microsoft.com/intellisense/ie3-2nav3-0");
    }
    else if( prjTargetSchema == 1)
    {
        wizardx.AddSymbol("DEFAULT_TARGET_SCHEMA", "http://schemas.microsoft.com/intellisense/ie5");
    }
    else
    {
        wizardx.AddSymbol("DEFAULT_TARGET_SCHEMA", "http://schemas.microsoft.com/intellisense/nav4-0");
    }
}

function AddDefaultDefaultHTMLPageLayoutToWizard(dtex, wizardx, selProj)
{
    var prjPageLayout = selProj.Properties("DefaultHTMLPageLayout").Value;
    // 0 = FlowLayout
    // 1 = GridLayout
    if(prjPageLayout == 0)
    {
        wizardx.AddSymbol("DEFAULT_HTML_LAYOUT", "FlowLayout");
    }
    else
    {
        wizardx.AddSymbol("DEFAULT_HTML_LAYOUT", "GridLayout");
    }
}
function AddDefaultWebFormsPropertiesToWizard(dtex, wizardx, selProj)
{
    AddDefaultClientScriptToWizard(dtex, wizardx, selProj);
    AddDefaultTargetSchemaToWizard(dtex, wizardx, selProj);
    AddDefaultDefaultHTMLPageLayoutToWizard(dtex, wizardx, selProj);
}


function IsReferenceExpanded(oProj)
{
    UIItem = GetUIReferencesNode(oProj);
    if( UIItem != null )
        return UIItem.Expanded;
}		

function ExpandReferencesNode(oProj)
{
    UIItem = GetUIReferencesNode(oProj);
    if( UIItem != null )
        UIItem.Expanded = true;
}

function CollapseReferencesNode(oProj)
{
    UIItem = GetUIReferencesNode(oProj);
    if( UIItem != null )
        UIItem.Expanded = false;
}

function GetUIReferencesNode(oProj)
{
    var L_strREferencesNode_Text = "引用"; // 此字符串需要本地化
    var UIItemX = null;

    UIItemX = GetUIItem( oProj, L_strREferencesNode_Text);
    if( UIItemX )
        return UIItemX.UIHierarchyItems;
    else
        return null;
}

//
// 返回输入层次结构项的父级。父级可以是文件夹、
// 上级项目或解决方案。
//
function getParent(obj)
{
    var parent = obj.Collection.parent;
    //
    // obj 是项目吗?
    //
    if( parent == dte )
    {
        //
        // obj 是子项目吗?
        //
        if( IsSubProject(obj) )
        {                
            parent = obj.ParentProjectItem.Collection.parent;
        }
        else
        {
            //
            // obj 是顶级项目
            //
            parent = null;
        }
    }
    return parent;    
}

function IsSubProject(oProj)
{
    try
    {
        var Parent = oProj.ParentProjectItem;
        if(Parent)
            return true;
        return false;
    }
    catch(e)
    {
        return false;
    }
}

//
// 获取项目项 sName 的 UIHierarchyItem。 
// 如果 sName 为空白，则返回项目的 UIHierarchyItem
//
function GetUIItem( oProj, sName )
{
    try
    {
        if( sName != "" )
        {
            sSaveName = sName;
            sName = oProj.Name + "\\" + sSaveName;
        }
        else
        {
            sName = oProj.Name;
        }

        var parent = getParent( oProj );

        while( parent != null )
        {
            sSaveName = sName;
            sName = parent.Name + "\\" + sSaveName;
            parent = getParent( parent );

        }

        //
        // 我们已经到达解决方案资源管理器层次结构的顶部 - 将 sName 索引返回到解决方案的 UIHierarchyItem 集合中
        //
        var strSolutionName = dte.Solution.Properties("Name");
        var vsHierObject = dte.Windows.Item(vsWindowKindSolutionExplorer).Object;   
        return vsHierObject.GetItem( strSolutionName + "\\" + sName );
    }	
    catch(e)
    {
        return null;
    }
}

//
// 查找项目上已存在的文件
//
function DoesFileExistInProj(oProj, sName )
{
    try
    {
        return oProj.ProjectItems.Item(sName);

    }	
    catch(e)
    {
        return null;
    }
}

function SetErrorInfo(error)
{
    if(error.description.length > 0)
    {
        wizard.SetErrorInfo(error.description, error.number & 0xFFFFFFFF);
    }
}

function ReportError( strErr )
{
    if( dte.SuppressUI == false )
    {
        wizard.ReportError(strErr);
    }
}

function ProjectIsARootWeb(strProjectPath)
{
    // 如果 strProjectPath 为根网站，则返回 true。这是通过对
    // 正斜杠进行计数来完成的。Web 根的形式为: http://server。假定
    // 结尾没有斜杠，则 Web 根将有 2 个正斜杠，非 Web 根将
    // 有 3 个或更多的斜杠。
    var nCntr = 0;
    var cSlashes = 0;
    var nLen = strProjectPath.length - 1;   // 忽略最后一个字符
    for (nCntr = 0; nCntr < nLen; nCntr++)
    {
        // 对正斜杠进行计数
        if(strProjectPath.charAt(nCntr) == "/")
            cSlashes++;
    }
    
    if(cSlashes == 2)
        return true;
    return false;
}
