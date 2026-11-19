// Copyright (c) Microsoft Corporation. All rights reserved.
// Script for MFC Add Simple Class

function OnFinish(selProj, selObj)
{
	var oCM;
	try
	{
		oCM	= selProj.CodeModel;

		var strClassName = wizard.FindSymbol("CLASS_NAME");

		var L_TransactionName_Text = "ÃÌº” MFC ºÚµ•¿‡ ";
		oCM.StartTransaction(L_TransactionName_Text + strClassName);

		var strProjectPath		= wizard.FindSymbol("PROJECT_PATH");
		var strTemplatePath		= wizard.FindSymbol("TEMPLATES_PATH");
		var strIncludeHeader	= wizard.FindSymbol("INCLUDE_HEADER");
		var strHeaderFile		= wizard.FindSymbol("HEADER_FILE");
		var strImplFile			= wizard.FindSymbol("IMPL_FILE");
		var strDialogID			= wizard.FindSymbol("IDD_DIALOG");
		var strTemplateHeader	= wizard.FindSymbol("TEMPLATE_HEADER");
		var strTemplateImpl		= wizard.FindSymbol("TEMPLATE_IMPL");
		var strHTMLID			= wizard.FindSymbol("HTML_ID");
		var strHTMLFile			= wizard.FindSymbol("HTML_FILE");
		var strBaseClass		= wizard.FindSymbol("BASE_CLASS");
		var bGenDocTemplate		= wizard.FindSymbol("GEN_DOCTEMPLATE");
		var bMDI				= wizard.FindSymbol("MDI_APP");
		var strDocumentClass	= wizard.FindSymbol("DOCUMENT_CLASS");
		var strFrameClass		= wizard.FindSymbol("FRAME_CLASS");
		var bAccessibility		= wizard.FindSymbol("ACCESSIBILITY");
		var strProjectCPP		= GetProjectFile(selProj, "CPP", false, true);

		var bDevice = IsDeviceProject(selProj);

		if (strIncludeHeader != "")
		{
			if (!DoesIncludeExist(selProj, strIncludeHeader, "stdafx.h"))
				oCM.AddInclude(strIncludeHeader, "stdafx.h", vsCMAddPositionEnd);
		}

		var strProjectRC = GetProjectFile(selProj, "RC", true);
		var oResHelper = wizard.ResourceHelper;

		if (wizard.FindSymbol("CREATE_DIALOG"))
		{
			var bDBCSCodePage =IsDBCSCharSet(wizard);

			var strRCTemplFile = strTemplatePath;
			if (!bDevice)
			{
				if (strBaseClass == "CDHtmlDialog")
					strRCTemplFile += (!bDBCSCodePage ? "\\dhtmldlg.rc" : "\\dhtmldlg_dbcs.rc");
				else if (strBaseClass == "CFormView")
					strRCTemplFile += (!bDBCSCodePage ? "\\formview.rc" : "\\formview_dbcs.rc");
				else
					strRCTemplFile += (!bDBCSCodePage ? "\\dialog.rc" : "\\dialog_dbcs.rc");

				oResHelper.OpenResourceFile(strProjectRC);
				var strSymbolValue = oResHelper.AddResource(strDialogID, strRCTemplFile, "DIALOG");
				if (strSymbolValue == null) return;				
				oResHelper.CloseResourceFile();
			}
			else
			{
				var configs = selProj.Object.Configurations;
				var completedResourceFiles = new Array();

				var ProjWiz = new ActiveXObject("ProjWiz.SDProjWiz2.2");
	
				for (var nCntr = 1; nCntr <= configs.Count; nCntr++)
				{
					var config = configs.Item(nCntr);
					var strCurrentResource = GetDeviceResourceFileForConfig(config);
					
					if (completedResourceFiles.join(";").indexOf(strCurrentResource) == -1)
					{
						var platformName = config.Platform.Name;
						var symbol = ProjWiz.GetBaseNativePlatformProperty(platformName, "UISymbol");
						var strPlatformRCTemplFile = strRCTemplFile;

						if (symbol == "POCKETPC2003_UI_MODEL")
						{
							if (strBaseClass == "CFormView")
								strPlatformRCTemplFile += (!bDBCSCodePage ? "\\formviewppc.rc" : "\\formview_dbcs_ppc.rc");
							else
								strPlatformRCTemplFile += (!bDBCSCodePage ? "\\dialogppc.rc" : "\\dialog_dbcs_ppc.rc");
						}
						else if (symbol == "SMARTPHONE2003_UI_MODEL")
						{
							if (strBaseClass == "CFormView")
								strPlatformRCTemplFile += (!bDBCSCodePage ? "\\formviewsp.rc" : "\\formview_dbcs_sp.rc");
							else
								strPlatformRCTemplFile += (!bDBCSCodePage ? "\\dialogsp.rc" : "\\dialog_dbcs_sp.rc");
						}
						else
						{
							if (strBaseClass == "CFormView")
								strPlatformRCTemplFile += (!bDBCSCodePage ? "\\formview.rc" : "\\formview_dbcs.rc");
							else
								strPlatformRCTemplFile += (!bDBCSCodePage ? "\\dialog.rc" : "\\dialog_dbcs.rc");
						}

						oResHelper.OpenResourceFile(strCurrentResource);
						var strSymbolValue = oResHelper.AddResource(strDialogID, strPlatformRCTemplFile, "DIALOG");
						oResHelper.CloseResourceFile();
						completedResourceFiles.push(strCurrentResource);				
					}
				}
			}

			wizard.AddSymbol("IDD_DIALOG", strSymbolValue.split("=").shift());
		}

		if (strBaseClass == "CDHtmlDialog")
		{
			RenderAddTemplate(wizard, "dhtmldlg.htm", strHTMLFile, selObj, false);
			oResHelper.OpenResourceFile(strProjectRC);
			var strSymbolValue = oResHelper.AddResource(strHTMLID, strProjectPath + strHTMLFile, "HTML");
			if (strSymbolValue == null) return;				
			oResHelper.CloseResourceFile();
			wizard.AddSymbol("HTML_ID", strSymbolValue.split("=").shift());
		}

		RenderAddTemplate(wizard, strTemplateHeader, strHeaderFile, selObj, true);
		RenderAddTemplate(wizard, strTemplateImpl, strImplFile, selObj, false);

		// look for InitInstance
		var oInitInstance = false;
		if ((strBaseClass == "CFormView" && bGenDocTemplate) || bAccessibility)
		{
			var oClasses = oCM.Classes;
			for (var nCntr = 1; nCntr <= oClasses.Count; nCntr++)
			{
				// look for class derived from CWinApp
				if (oClasses(nCntr).IsDerivedFrom("CWinApp"))
				{
					oInitInstance = oClasses(nCntr).Functions.Find("InitInstance");
					break;
				}
			}
		}

		// insert AfxOleInit()
		if (bAccessibility && oInitInstance)
		{
			var strBody = oInitInstance.BodyText;
			if (-1 == strBody.indexOf("AfxOleInit"))
			{
				oInitInstance.StartPointOf(vsCMPartBody, vsCMWhereDefinition).CreateEditPoint().Insert("\tAfxOleInit();\r\n");
				oCM.Synchronize();						
			}
		}

		if (strBaseClass == "CFormView" && bGenDocTemplate)
		{
			var strMainFrameCaption = wizard.FindSymbol("DOCUMENT_MAIN_FRAME_CAPTION");
			var strFileNewNameShort = wizard.FindSymbol("DOCUMENT_FILE_NEW_NAME_SHORT");
			var strTypeName = wizard.FindSymbol("DOCUMENT_TYPE_NAME");
			var strFilterName = wizard.FindSymbol("DOCUMENT_FILTER_NAME");
			var strFileExt = wizard.FindSymbol("DOCUMENT_FILE_EXTENSION");
			var strFileTypeID = wizard.FindSymbol("DOCUMENT_FILE_TYPE_ID");
			var strFileNewNameLong = wizard.FindSymbol("DOCUMENT_FILE_NEW_NAME_LONG");
			var strTemp = strMainFrameCaption + "\\n" + strFileNewNameShort + "\\n" + strTypeName + "\\n" + strFilterName + "\\n." + strFileExt + "\\n" + strFileTypeID + "\\n" + strFileNewNameLong;

			var strID = CreateSafeName(strClassName).toUpperCase();
			if (strID.charAt(0) == "C")
				strID = strID.substr(1);
			strID = "IDR_" + strID + "_TMPL";
	
			if (!bDevice)
			{
				oResHelper.OpenResourceFile(strProjectRC);			

				var strIconFile;
				if (strClassName.charAt(0) == "C" || strClassName.charAt(0) == "c")
					strIconFile = strClassName.substr(1) + ".ico";
				else
					strIconFile = strClassName + ".ico";
				strIconFile = GetUniqueFileName(strProjectPath, strIconFile);
				wizard.RenderTemplate(strTemplatePath + "\\formview.ico", strProjectPath + "res\\" + strIconFile, true); //don't process ico file
				var strSymbolValue = oResHelper.AddResource(strID, strProjectPath + "res\\" + strIconFile, "ICON", "", true);
				if (strSymbolValue == null) return;				
				strID = strSymbolValue.split("=").shift();

				var strSymbolValue2 = oResHelper.AddResource(strID, strTemp, "STRING", "", true);
				if (strSymbolValue2 == null) return;				

				if (bMDI)
					strSymbolValue2 = oResHelper.AddResource(strID, strTemplatePath + "\\formviewmdi.rc", "MENU", "#include <afxres.h>", true);
				else
					strSymbolValue2 = oResHelper.AddResource(strID, strTemplatePath + "\\formviewsdi.rc", "MENU", "#include <afxres.h>", true);
				if (strSymbolValue2 == null) return;				

				oResHelper.CloseResourceFile();
			}
			else
			{
				var configs = selProj.Object.Configurations;
				var completedResourceFiles = new Array();

				var ProjWiz = new ActiveXObject("ProjWiz.SDProjWiz2.2");
	
				for (var nCntr = 1; nCntr <= configs.Count; nCntr++)
				{
					var config = configs.Item(nCntr);
					var strCurrentResource = GetDeviceResourceFileForConfig(config);
					
					if (completedResourceFiles.join(";").indexOf(strCurrentResource) == -1)
					{
						var platformName = config.Platform.Name;
						var symbol = ProjWiz.GetBaseNativePlatformProperty(platformName, "UISymbol");

						oResHelper.OpenResourceFile(strCurrentResource);			

						var strIconFile;
						if (strClassName.charAt(0) == "C" || strClassName.charAt(0) == "c")
							strIconFile = strClassName.substr(1) + ".ico";
						else
							strIconFile = strClassName + ".ico";
						strIconFile = GetUniqueFileName(strProjectPath, strIconFile);
						wizard.RenderTemplate(strTemplatePath + "\\formview.ico", strProjectPath + "res\\" + strIconFile, true); //don't process ico file
						var strSymbolValue = oResHelper.AddResource(strID, strProjectPath + "res\\" + strIconFile, "ICON", "", true);
						if (strSymbolValue == null) return;				
						strID = strSymbolValue.split("=").shift();

						var strSymbolValue2 = oResHelper.AddResource(strID, strTemp, "STRING", "", true);
						if (strSymbolValue == null) return;				

						// We won't add MENU resource for Pocket PC and Smartphone apps since they would also need the 
						// corresponding RCDATA resource in order to work. RCDATA resource would be mangled by resource editor 
						// into unreadable hexadecimal data if we put it into .rc file instead of .rc2 file. So we inclined NOT
						// to add MENU resource unless it's Windows CE apps.
						if (symbol == "STANDARDSHELL_UI_MODEL")
						{
							oResHelper.AddResource(strID, strTemplatePath + "\\formviewsdi.rc", "MENU", "#include <afxres.h>", true);
						}
						if (strSymbolValue2 == null) return;				

						oResHelper.CloseResourceFile();
						completedResourceFiles.push(strCurrentResource);				
					}
				}
			}

			if (oInitInstance)
			{
				if (!DoesIncludeExist(selProj, strHeaderFile, strProjectCPP))
					oCM.AddInclude('"' + strHeaderFile + '"', strProjectCPP, vsCMAddPositionEnd);

				var	strInsert = "\t{\r\n";
				if (bMDI)
					strInsert += "\t\tCMultiDocTemplate* pNewDocTemplate = new CMultiDocTemplate(\r\n";
				else
					strInsert += "\t\tCSingleDocTemplate* pNewDocTemplate = new CSingleDocTemplate(\r\n";
				strInsert += "\t\t\t" + strID + ",\r\n";
				strInsert += "\t\t\tRUNTIME_CLASS(" + strDocumentClass + "),\r\n";
				strInsert += "\t\t\tRUNTIME_CLASS(" + strFrameClass + "),\r\n";
				strInsert += "\t\t\tRUNTIME_CLASS(" + strClassName + "));\r\n";
				strInsert += "\t\tAddDocTemplate(pNewDocTemplate);\r\n";
				strInsert += "\t}\r\n\r\n";
				oInitInstance.StartPointOf(vsCMPartBody, vsCMWhereDefinition).CreateEditPoint().Insert(strInsert);
				oCM.Synchronize();						
			}
		}

		if (wizard.FindSymbol("AUTOMATION") || wizard.FindSymbol("CREATABLE"))
		{
			var strProjectIDL = GetProjectFile(selProj, "IDL");
			if (!strProjectIDL || strProjectIDL.length == 0)
				strProjectIDL = GetProjectFile(selProj, "ODL");
			if (strProjectIDL.length)
			{
				// Add #include "olectl.h" to strProject.idl
				if (!DoesIncludeExist(selProj, '"olectl.h"', strProjectIDL))
					oCM.AddInclude('"olectl.h"', strProjectIDL, vsCMAddPositionEnd);

				// Render coclass.idl and insert into strProject.idl
				AddCoclassFromFile(oCM, "coclass.idl");
			}
		}

		
		oCM.CommitTransaction();
				
		var newClass = oCM.Classes.Find(strClassName);
		if(newClass)
			newClass.StartPoint.TryToShow(vsPaneShowTop);
		
	}
	catch(e)
	{
		if (oCM)
			oCM.AbortTransaction();

		if (e.description.length != 0)
			SetErrorInfo(e);
		return e.number
	}
}
