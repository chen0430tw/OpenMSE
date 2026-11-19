// Copyright (c) Microsoft Corporation. All rights reserved.
// Script for ATL Property Page

function OnFinish(selProj, selObj)
{
	var oCM;
	try
	{
		oCM	= selProj.CodeModel;
		var bDevice  = IsDeviceProject(selProj);

		wizard.AddSymbol("DEVICE", bDevice);
		
		// used only for device projects where platforms may not support DCOM.
		wizard.AddSymbol("SUPPORT_DCOM", false);
		wizard.AddSymbol("SUPPORT_NON_DCOM", false);

		if (!bDevice)
		{
			var strShortName = wizard.FindSymbol("SHORT_NAME");
			var L_TRANSACTION_Text = "添加 ATL 属性页 ";
			oCM.StartTransaction(L_TRANSACTION_Text + strShortName);
			if(!AddATLSupportToProject(selProj))
			{
				oCM.AbortTransaction();
				return;
			}

			var bDLL;
			if (typeDynamicLibrary == selProj.Object.Configurations(1).ConfigurationType)
				bDLL = true;
			else
				bDLL = false;
			wizard.AddSymbol("DLL_APP", bDLL);

			var strProjectName		= wizard.FindSymbol("PROJECT_NAME");
			wizard.AddSymbol("SAFE_IDL_NAME", CreateASCIIName(wizard.FindSymbol("PROJECT_NAME")));
			var strProjectPath		= wizard.FindSymbol("PROJECT_PATH");
			var strTemplatePath		= wizard.FindSymbol("TEMPLATES_PATH");
			var strUpperShortName	= CreateASCIIName(strShortName.toUpperCase());
			strUpperShortName		= CreateSafeName(strUpperShortName); 
			wizard.AddSymbol("UPPER_SHORT_NAME", strUpperShortName);
			var strVIProgID			= wizard.FindSymbol("VERSION_INDEPENDENT_PROGID");
			wizard.AddSymbol("PROGID", strVIProgID.substr(0,37) + ".1");
			var strClassName		= wizard.FindSymbol("CLASS_NAME");
			var strHeaderFile		= wizard.FindSymbol("HEADER_FILE");
			var strImplFile			= wizard.FindSymbol("IMPL_FILE");
			var strCoClass			= wizard.FindSymbol("COCLASS");
			var bAttributed			= wizard.FindSymbol("ATTRIBUTED");

			var strProjectRC		= GetProjectFile(selProj, "RC", true, false);

			var strProjectH			= GetProjectFile(selProj, "H", false, IsMFCProject(selProj,false));
			wizard.AddSymbol("PROJECT_HEADER_NAME", strProjectH);

			// Create necessary GUIDS
			CreateGUIDs();

			// open resource file
			var oResHelper = wizard.ResourceHelper;
			oResHelper.OpenResourceFile(strProjectRC);

			if (!bAttributed)
			{
				// Get LibName
				wizard.AddSymbol("LIB_NAME", oCM.IDLLibraries(1).Name);

				// Get LibID
				wizard.AddSymbol("LIBID_REGISTRY_FORMAT", oCM.IDLLibraries(1).Attributes.Find("uuid").Value);

				// Get AppID
				var strAppID = wizard.GetAppID();
				if (strAppID.length > 0)
				{
					wizard.AddSymbol("APPID_EXIST", true);
					wizard.AddSymbol("APPID_REGISTRY_FORMAT", strAppID);
				}

				// Render proppageco.idl and insert into strProject.idl
				AddCoclassFromFile(oCM, "proppageco.idl");

				SetMergeProxySymbol(selProj);
			}

			var strDLGID = "IDD_" + strUpperShortName;
			var bDBCSCodePage = IsDBCSCharSet(wizard);
			var strRCTemplate = strTemplatePath + (!bDBCSCodePage ? "\\propdlg.rc" : "\\prp_dbcs.rc");
			var strRCTemplFile = RenderToTemporaryResourceFile(strRCTemplate);
			var strSymbolValue = oResHelper.AddResource(strDLGID, strRCTemplFile, "DIALOG");
			if (strSymbolValue == null) return;				
			wizard.AddSymbol("IDD_DIALOGID", strSymbolValue.split("=").shift());

			var strTitleID = "IDS_TITLE" + strUpperShortName;
			strSymbolValue = oResHelper.AddResource(strTitleID, wizard.FindSymbol("TITLE"), "STRING");
			if (strSymbolValue == null) return;				
			wizard.AddSymbol("IDS_TITLE", strSymbolValue.split("=").shift());

			var strHelpFileID = "IDS_HELPFILE" + strUpperShortName;
			strSymbolValue = oResHelper.AddResource(strHelpFileID, wizard.FindSymbol("HELP_FILE"), "STRING");
			if (strSymbolValue == null) return;				
			wizard.AddSymbol("IDS_HELPFILE", strSymbolValue.split("=").shift());

			var strDocStringID = "IDS_DOCSTRING" + strUpperShortName;
			strSymbolValue = oResHelper.AddResource(strDocStringID, wizard.FindSymbol("DOC_STRING"), "STRING");
			if (strSymbolValue == null) return;				
			wizard.AddSymbol("IDS_DOCSTRING", strSymbolValue.split("=").shift());

			// Add #include <atlcom.h> to stdafx.h
			if (!DoesIncludeExist(selProj, '<atlcom.h>', "stdafx.h"))
				oCM.AddInclude('<atlcom.h>', "stdafx.h", vsCMAddPositionEnd);
			// Add #include <atlctl.h> to stdafx.h
			if (!DoesIncludeExist(selProj, '<atlctl.h>', "stdafx.h"))
				oCM.AddInclude('<atlctl.h>', "stdafx.h", vsCMAddPositionEnd);


			if (!bAttributed)
			{
				// add RGS file resource
				var strRGSFile = GetUniqueFileName(strProjectPath, CreateASCIIName(strShortName) + ".rgs");
				RenderAddTemplate(wizard, "proppage.rgs", strRGSFile, false, false);
				var strRGSID = "IDR_" + strUpperShortName;
				strSymbolValue = oResHelper.AddResource(strRGSID, strProjectPath + strRGSFile, "REGISTRY");
				if (strSymbolValue == null) return;				
				wizard.AddSymbol("RGS_ID", strSymbolValue.split("=").shift());
			}
			// close resource file
			oResHelper.CloseResourceFile();
		}
		else
		{
			var strShortName = wizard.FindSymbol("SHORT_NAME");
			var L_TRANSACTION_Text = "添加 ATL 属性页 ";
			oCM.StartTransaction(L_TRANSACTION_Text + strShortName);
			if(!AddATLSupportToProject(selProj))
			{
				oCM.AbortTransaction();
				return;
			}

			var bDLL;
			if (typeDynamicLibrary == selProj.Object.Configurations(1).ConfigurationType)
				bDLL = true;
			else
				bDLL = false;
			wizard.AddSymbol("DLL_APP", bDLL);

			var strProjectName		= wizard.FindSymbol("PROJECT_NAME");
			wizard.AddSymbol("SAFE_IDL_NAME", CreateASCIIName(wizard.FindSymbol("PROJECT_NAME")));
			var strProjectPath		= wizard.FindSymbol("PROJECT_PATH");
			var strTemplatePath		= wizard.FindSymbol("TEMPLATES_PATH");
			var strUpperShortName		= strShortName.toUpperCase();
			strUpperShortName		= CreateSafeName(strUpperShortName); 
			wizard.AddSymbol("UPPER_SHORT_NAME", strUpperShortName);
			var strVIProgID			= wizard.FindSymbol("VERSION_INDEPENDENT_PROGID");
			wizard.AddSymbol("PROGID", strVIProgID.substr(0,37) + ".1");
			var strClassName		= wizard.FindSymbol("CLASS_NAME");
			var strHeaderFile		= wizard.FindSymbol("HEADER_FILE");
			var strImplFile			= wizard.FindSymbol("IMPL_FILE");
			var strCoClass			= wizard.FindSymbol("COCLASS");
			var bAttributed			= wizard.FindSymbol("ATTRIBUTED");
			var strProjectH			= GetProjectFile(selProj, "H", false, IsMFCProject(selProj,false));
			wizard.AddSymbol("PROJECT_HEADER_NAME", strProjectH);
			var strRGSFile = GetUniqueFileName(strProjectPath, CreateASCIIName(strShortName) + ".rgs");
			var strRGSID = "IDR_" + strUpperShortName;
			var strRGSDCOMFile = GetUniqueFileName(strProjectPath, CreateASCIIName(strShortName + "DCOM") + ".rgs");;
			var strRGSDCOMID = "IDR_" + strUpperShortName + "DCOM";

			// Create necessary GUIDS
			CreateGUIDs();

			if (!bAttributed)
			{
				// Get LibName
				wizard.AddSymbol("LIB_NAME", oCM.IDLLibraries(1).Name);

				// Get LibID
				wizard.AddSymbol("LIBID_REGISTRY_FORMAT", oCM.IDLLibraries(1).Attributes.Find("uuid").Value);

				// Get AppID
				var strAppID = wizard.GetAppID();
				if (strAppID.length > 0)
				{
					wizard.AddSymbol("APPID_EXIST", true);
					wizard.AddSymbol("APPID_REGISTRY_FORMAT", strAppID);
				}

				// Render proppageco.idl and insert into strProject.idl
				AddCoclassFromFile(oCM, "proppageco.idl");

				SetMergeProxySymbol(selProj);
			}


			// Add #include <atlcom.h> to stdafx.h
			if (!DoesIncludeExist(selProj, '<atlcom.h>', "stdafx.h"))
				oCM.AddInclude('<atlcom.h>', "stdafx.h", vsCMAddPositionEnd);
			// Add #include <atlctl.h> to stdafx.h
			if (!DoesIncludeExist(selProj, '<atlctl.h>', "stdafx.h"))
				oCM.AddInclude('<atlctl.h>', "stdafx.h", vsCMAddPositionEnd);

			var bDeviceDCOM = ProjectContainsDCOMPlatform(selProj)
			var bDeviceNonDCOM = ProjectContainsNonDCOMPlatform(selProj)


			if (!bAttributed)
			{
				// add RGS file resource
				if (bDeviceDCOM)
				{
					wizard.AddSymbol("SUPPORT_DCOM", true);
					RenderAddTemplate(wizard, "proppage.rgs", strRGSDCOMFile, false, false);
					wizard.AddSymbol("SUPPORT_DCOM", false);
				}
				if (bDeviceNonDCOM)
				{
					wizard.AddSymbol("SUPPORT_NON_DCOM", true);
					RenderAddTemplate(wizard, "proppage.rgs", strRGSFile, false, false);
					wizard.AddSymbol("SUPPORT_NON_DCOM", false);
				}
				var strRGSID = "IDR_" + strUpperShortName;
			}

			wizard.AddSymbol("SUPPORT_DCOM", bDeviceDCOM);
			wizard.AddSymbol("SUPPORT_NON_DCOM", bDeviceNonDCOM);

			var strDLGID = "IDD_" + strUpperShortName;
			var bDBCSCodePage = IsDBCSCharSet(wizard);

			var configs = selProj.Object.Configurations;
			var completedResourceFiles = new Array();
			AddDeviceSymbols(false);

			var ProjWiz = new ActiveXObject("ProjWiz.SDProjWiz2.2");
			var oResHelper = wizard.ResourceHelper;

			for (var nCntr = 1; nCntr <= configs.Count; nCntr++)
			{
				var config = configs.Item(nCntr);
				var strCurrentResource = GetDeviceResourceFileForConfig(config);

				if (completedResourceFiles.join(";").indexOf(strCurrentResource) == -1)
				{
					var platformName = config.Platform.Name;
					var symbol = ProjWiz.GetBaseNativePlatformProperty(platformName, "UISymbol");

					// open resource file
					oResHelper.OpenResourceFile(strCurrentResource);

					// before we add the file to the project, render it to a temporary file. Then we 
					// can add the resulting preprocessed file to the current resource file. At which
					// point we delete the temporary file.
					var strRCTemplate = strTemplatePath;
					if (symbol == "POCKETPC2003_UI_MODEL")
					{
						strRCTemplate += (!bDBCSCodePage ? "\\propdlgppc.rc" : "\\prp_dbcs_ppc.rc");
					}
					else if (symbol == "SMARTPHONE2003_UI_MODEL")
					{
						strRCTemplate += (!bDBCSCodePage ? "\\propdlgsp.rc" : "\\prp_dbcs_sp.rc");
					}
					else
					{
						strRCTemplate += (!bDBCSCodePage ? "\\propdlgce.rc" : "\\prp_dbcs_ce.rc");
					}

					var strRCTemplFile = RenderToTemporaryResourceFile(strRCTemplate);

					var strCurrentSymbol = GetDeviceSymbolForConfig(config);
					wizard.AddSymbol(strCurrentSymbol, true);
					var strSymbolValue = oResHelper.AddResource(strDLGID, strRCTemplFile, "DIALOG");		
					if (strSymbolValue == null) return;				
					wizard.AddSymbol("IDD_DIALOGID", strSymbolValue.split("=").shift());

					var strTitleID = "IDS_TITLE" + strShortName;
					strSymbolValue = oResHelper.AddResource(strTitleID, wizard.FindSymbol("TITLE"), "STRING");
					if (strSymbolValue == null) return;				
					wizard.AddSymbol("IDS_TITLE", strSymbolValue.split("=").shift());

					var strHelpFileID = "IDS_HELPFILE" + strShortName;
					strSymbolValue = oResHelper.AddResource(strHelpFileID, wizard.FindSymbol("HELP_FILE"), "STRING");
					if (strSymbolValue == null) return;				
					wizard.AddSymbol("IDS_HELPFILE", strSymbolValue.split("=").shift());

					var strDocStringID = "IDS_DOCSTRING" + strShortName;
					strSymbolValue = oResHelper.AddResource(strDocStringID, wizard.FindSymbol("DOC_STRING"), "STRING");
					if (strSymbolValue == null) return;				
					wizard.AddSymbol("IDS_DOCSTRING", strSymbolValue.split("=").shift());
					
					if (!bAttributed)
					{
						if (bDeviceNonDCOM)
						{
							strSymbolValue = oResHelper.AddResource(strRGSID, strProjectPath + strRGSFile, "REGISTRY");
							if (strSymbolValue == null) return;				
							wizard.AddSymbol("RGS_ID", strSymbolValue.split("=").shift());	   
						}
						if (bDeviceDCOM)
						{
							strSymbolValue = oResHelper.AddResource(strRGSDCOMID, strProjectPath + strRGSDCOMFile, "REGISTRY");
							if (strSymbolValue == null) return;				
							wizard.AddSymbol("RGSDCOM_ID", strSymbolValue.split("=").shift());	
						}
					}

					// close resource file
					oResHelper.CloseResourceFile();
					completedResourceFiles.push(strCurrentResource);
					wizard.AddSymbol(strCurrentSymbol, false);
				}
			}
		}

		// Add header
		RenderAddTemplate(wizard, "proppage.h", strHeaderFile, selObj, true);

		// Add CPP
		RenderAddTemplate(wizard, "proppage.cpp", strImplFile, selObj, false);

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
		throw e;
	}
}
