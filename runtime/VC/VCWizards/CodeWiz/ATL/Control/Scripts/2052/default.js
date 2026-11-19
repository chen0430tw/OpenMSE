// Copyright (c) Microsoft Corporation. All rights reserved.
// Script for ATL Controls

function OnFinish(selProj, selObj)
{
	var oCM;
	try
	{
		oCM	= selProj.CodeModel;
		
		var bDevice = IsDeviceProject(selProj);
		wizard.AddSymbol("DEVICE", bDevice);
		
		// used only for device projects where platforms may not support DCOM.
		wizard.AddSymbol("SUPPORT_DCOM", false);
		wizard.AddSymbol("SUPPORT_NON_DCOM", false);

		var strShortName = wizard.FindSymbol("SHORT_NAME");
		var L_TRANSACTION_Text = "Ìí¼Ó ATL ¿Ø¼þ ";
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
		var strInterfaceName	= wizard.FindSymbol("INTERFACE_NAME");
		var strHTMLID 			= "IDH_" + strUpperShortName;
		var strDLGID  			= "IDD_" + strUpperShortName;
		wizard.AddSymbol("UPPER_SHORT_NAME", strUpperShortName);
		var strVIProgID			= wizard.FindSymbol("VERSION_INDEPENDENT_PROGID");
		wizard.AddSymbol("PROGID", strVIProgID.substr(0,37) + ".1");
		var bConnectionPoint	= wizard.FindSymbol("CONNECTION_POINTS");
 		var strClassName		= wizard.FindSymbol("CLASS_NAME");
		var strHeaderFile		= wizard.FindSymbol("HEADER_FILE");
		var strImplFile			= wizard.FindSymbol("IMPL_FILE");
		var strCoClass			= wizard.FindSymbol("COCLASS");
		var bAttributed			= wizard.FindSymbol("ATTRIBUTED");

		var bHTMLCtl			= wizard.FindSymbol("HTML_CONTROL");
		var bCompositeCtl		= wizard.FindSymbol("COMPOSITE_CONTROL");

		var strProjectRC		= GetProjectFile(selProj, "RC", true, false);
		var strProjectIDL		= GetProjectFile(selProj, "IDL", false, false);

		if (strProjectRC == "")
		{
			var L_MissingResourceError = "Your have to add a resource file to your project before you can add ATL Controls to it";
			wizard.ReportError(L_MissingResourceError);
			return;
		}
		// Create necessary GUIDS
		CreateGUIDs();

		if (!bDevice)
		{
			// open resource file
			var oResHelper = wizard.ResourceHelper;
			oResHelper.OpenResourceFile(strProjectRC);

			// Add Bitmap resource
			var strBitmapFile = GetUniqueFileName(strProjectPath, strShortName + ".bmp");
			var strBMPID = "IDB_" + strUpperShortName;
			wizard.RenderTemplate(strTemplatePath + "\\" + "toolbar.bmp", strProjectPath + strBitmapFile, true); //don't process bitmap
			var strNameAndID = oResHelper.AddResource(strBMPID, strProjectPath + strBitmapFile, "BITMAP");
			if (strNameAndID == null) return;
			var nEqualPos = strNameAndID.indexOf("=");
			var strSymbolName = strNameAndID.substr(0, nEqualPos);
			var strSymbolID = strNameAndID.substr(nEqualPos + 1);
			wizard.AddSymbol("IDR_BMPID_VALUE", strSymbolID);

			if (!bAttributed)
			{
				// Get LibName
				wizard.AddSymbol("LIB_NAME", oCM.IDLLibraries(1).Name);

				// Get LibID
				var oUuid = oCM.IDLLibraries(1).Attributes.Find("uuid");
				if (oUuid)
					wizard.AddSymbol("LIBID_REGISTRY_FORMAT", oUuid.Value);

				// Get typelib version
				var oVersion = oCM.IDLLibraries(1).Attributes.Find("version");
				if (oVersion)
				{
					var aryMajorMinor = oVersion.Value.split('.');
					for (var nCntr=0; nCntr<aryMajorMinor.length; nCntr++)
					{
						if (nCntr == 0)
							wizard.AddSymbol("TYPELIB_VERSION_MAJOR", aryMajorMinor[nCntr]);
						else
							wizard.AddSymbol("TYPELIB_VERSION_MINOR", aryMajorMinor[nCntr]);
					}
				}

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
				RenderAddTemplate(wizard, "control.rgs", strRGSFile, false, false);
				var strSymbolValue = oResHelper.AddResource(strRGSID, strProjectPath + strRGSFile, "REGISTRY");
				if (strSymbolValue == null) return;				
				wizard.AddSymbol("RGS_ID", strSymbolValue.split("=").shift());

				// Add connection point support
				if (bConnectionPoint)
					RenderAddTemplate(wizard, "connpt.h", "_" + strInterfaceName + "Events_CP.h", selObj, false);

				// Add #include "olectl.h" to strProject.idl
				if (!DoesIncludeExist(selProj, '"olectl.h"', strProjectIDL))
					oCM.AddInclude('"olectl.h"', strProjectIDL, vsCMAddPositionEnd);

				// Render ctlco.idl and insert into strProject.idl
				AddCoclassFromFile(oCM, "ctlco.idl");

				// Render ctlint.idl and insert into strProject.idl
				AddInterfaceFromFile(oCM, "ctlint.idl");

				SetMergeProxySymbol(selProj);
			}

			if (bHTMLCtl)
			{
				var strHTMLFile = GetUniqueFileName(strProjectPath, strShortName + "UI.htm");
				RenderAddTemplate(wizard, "HTMLCTL.htm", strHTMLFile, false, false);
				var strSymbolValue = oResHelper.AddResource(strHTMLID, strProjectPath + strHTMLFile, "HTML");
				if (strSymbolValue == null) return;								
				wizard.AddSymbol("IDH_HTMLID", strSymbolValue.split("=").shift());
			}

			if (bCompositeCtl)
			{
				var bDBCSCodePage = IsDBCSCharSet(wizard);
				var strRCTemplFile = strTemplatePath + (!bDBCSCodePage ? "\\cmposite.rc" : "\\cmp_dbcs.rc");
				var strTemporaryResourceFile = RenderToTemporaryResourceFile(strRCTemplFile);
				var strSymbolValue = oResHelper.AddResource(strDLGID, strTemporaryResourceFile, "DIALOG");
				if (strSymbolValue == null) return;				
				wizard.AddSymbol("IDD_DIALOGID", strSymbolValue.split("=").shift());
			}
			
			// Add header
			RenderAddTemplate(wizard, "control.h", strHeaderFile, selObj, true);	

			// Add HTML
			var strHTMLFile = GetUniqueFileName(strProjectPath, strShortName + ".htm");
			RenderAddTemplate(wizard, "default.htm", strHTMLFile, selObj, false);

			// Add CPP
			RenderAddTemplate(wizard, "control.cpp", strImplFile, selObj, false);

			// close resource file
			oResHelper.CloseResourceFile();
		}
		else
		{
		
			var bFirst = true;

			var oUuid;
			var oVersion;
			var aryMajorMinor;
			var strAppID;
			// add RGS file resource
			var strRGSFile = GetUniqueFileName(strProjectPath, CreateASCIIName(strShortName) + ".rgs");
			var strRGSDCOMFile = GetUniqueFileName(strProjectPath, CreateASCIIName(strShortName + "DCOM") + ".rgs");
			var strRGSID = "IDR_" + strUpperShortName;
			var strRGSDCOMID = "IDR_" + strUpperShortName + "DCOM";
			var strHTMLFile;
			var bDeviceDCOM = ProjectContainsDCOMPlatform(selProj)
			var bDeviceNonDCOM = ProjectContainsNonDCOMPlatform(selProj)

			if (!bAttributed)
			{
				// Get LibName
				wizard.AddSymbol("LIB_NAME", oCM.IDLLibraries(1).Name);

				// Get LibID
				oUuid = oCM.IDLLibraries(1).Attributes.Find("uuid");
				if (oUuid)
					wizard.AddSymbol("LIBID_REGISTRY_FORMAT", oUuid.Value);

				// Get typelib version
				oVersion = oCM.IDLLibraries(1).Attributes.Find("version");
				if (oVersion)
				{
					aryMajorMinor = oVersion.Value.split('.');
					for (var nCntr=0; nCntr<aryMajorMinor.length; nCntr++)
					{
						if (nCntr == 0)
							wizard.AddSymbol("TYPELIB_VERSION_MAJOR", aryMajorMinor[nCntr]);
						else
							wizard.AddSymbol("TYPELIB_VERSION_MINOR", aryMajorMinor[nCntr]);
					}
				}

				// Get AppID
				var strAppID = wizard.GetAppID();
				if (strAppID.length > 0)
				{
					wizard.AddSymbol("APPID_EXIST", true);
					wizard.AddSymbol("APPID_REGISTRY_FORMAT", strAppID);
				}
			}
			if (bHTMLCtl)
			{
				strHTMLFile = GetUniqueFileName(strProjectPath, strShortName + "UI.htm");
				RenderAddTemplate(wizard, "HTMLCTL.htm", strHTMLFile, false, false);
			}

			if (bDeviceNonDCOM)
			{
				wizard.AddSymbol("SUPPORT_NON_DCOM", true);
				RenderAddTemplate(wizard, "control.rgs", strRGSFile, false, false);
				wizard.AddSymbol("SUPPORT_NON_DCOM", false);

			}
			if (bDeviceDCOM)
			{
				wizard.AddSymbol("SUPPORT_DCOM", true);
				RenderAddTemplate(wizard, "control.rgs", strRGSDCOMFile, false, false);
				wizard.AddSymbol("SUPPORT_DCOM", false);	
			}

			var bDBCSCodePage = IsDBCSCharSet(wizard);

			var configs = selProj.Object.Configurations;
			var completedResourceFiles = new Array();
			AddDeviceSymbols(false);
			wizard.AddSymbol("SUPPORT_NON_DCOM", bDeviceNonDCOM);
			wizard.AddSymbol("SUPPORT_DCOM", bDeviceDCOM);

			var ProjWiz = new ActiveXObject("ProjWiz.SDProjWiz2.2");
			var oResHelper = wizard.ResourceHelper;

			for (var nCntr = 1; nCntr <= configs.Count; nCntr++)
			{
				var config = configs.Item(nCntr);
				var strCurrentResource = GetDeviceResourceFileForConfig(config);

				if (completedResourceFiles.join(";").indexOf(strCurrentResource) == -1)
				{
					// open resource file
					oResHelper.OpenResourceFile(strCurrentResource);

					if (!bAttributed)
					{
						// TODO: add checks that verify the RGS ID is the same across all .rc/.h files...
						// add RGS file resource
						if (bFirst)
						{
							if (bDeviceNonDCOM)
							{
								var strSymbolValue = oResHelper.AddResource(strRGSID, strProjectPath + strRGSFile, "REGISTRY");					
								if (strSymbolValue == null) return;											
								wizard.AddSymbol("RGS_ID", strSymbolValue.split("=").shift());
							}
							if (bDeviceDCOM)
							{
								var strSymbolValue = oResHelper.AddResource(strRGSDCOMID, strProjectPath + strRGSDCOMFile, "REGISTRY");					
								if (strSymbolValue == null) return;											
								wizard.AddSymbol("RGSDCOM_ID", strSymbolValue.split("=").shift());	
							}
							// Add connection point support
							if (bConnectionPoint)
								RenderAddTemplate(wizard, "connpt.h", "_" + strInterfaceName + "Events_CP.h", selObj, false);

							// Add #include "olectl.h" to strProject.idl
							if (!DoesIncludeExist(selProj, '"olectl.h"', strProjectIDL))
								oCM.AddInclude('"olectl.h"', strProjectIDL, vsCMAddPositionEnd);

							// Render ctlco.idl and insert into strProject.idl
							AddCoclassFromFile(oCM, "ctlco.idl");

							// Render ctlint.idl and insert into strProject.idl
							AddInterfaceFromFile(oCM, "ctlint.idl");

							SetMergeProxySymbol(selProj);
						}
						else
						{
							// See above TODO
							if (bDeviceNonDCOM)
							{
							    var strSymbolValue = oResHelper.AddResource(strRGSID, strProjectPath + strRGSFile, "REGISTRY");
							    if (strSymbolValue == null) return;
							}
							if (bDeviceDCOM)
							{
							    var strSymbolValue = oResHelper.AddResource(strRGSDCOMID, strProjectPath + strRGSDCOMFile, "REGISTRY");
							    if (strSymbolValue == null) return;
							}		
						}

					}

					if (bHTMLCtl)
					{
						if (bFirst)
						{
							var strSymbolValue = oResHelper.AddResource(strHTMLID, strProjectPath + strHTMLFile, "HTML");
							if (strSymbolValue == null) return;				
							wizard.AddSymbol("IDH_HTMLID", strSymbolValue.split("=").shift());
						}
						else
						{
							// See above TODO
							var strSymbolValue = oResHelper.AddResource(strHTMLID, strProjectPath + strHTMLFile, "HTML");
							if (strSymbolValue == null) return;				
						}
					}

					if (bCompositeCtl)
					{
						var platformName = config.Platform.Name;
						var symbol = ProjWiz.GetBaseNativePlatformProperty(platformName, "UISymbol");

						var strRCTemplFile = strTemplatePath;
						if (symbol == "POCKETPC2003_UI_MODEL")
						{
							 strRCTemplFile += (!bDBCSCodePage ? "\\cmpositeppc.rc" : "\\cmp_dbcs_ppc.rc");
						}
						else if (symbol == "SMARTPHONE2003_UI_MODEL")
						{
							 strRCTemplFile += (!bDBCSCodePage ? "\\cmpositesp.rc" : "\\cmp_dbcs_sp.rc");
						}
						else
						{
							 strRCTemplFile += (!bDBCSCodePage ? "\\cmpositece.rc" : "\\cmp_dbcs_ce.rc");
						}
						
						var strTemporaryResourceFile =  RenderToTemporaryResourceFile(strRCTemplFile);
						if (bFirst)
						{
							var strSymbolValue = oResHelper.AddResource(strDLGID, strTemporaryResourceFile, "DIALOG");
							if (strSymbolValue == null) return;				
							wizard.AddSymbol("IDD_DIALOGID", strSymbolValue.split("=").shift());
						}
						else
						{
							// see above TODO
							var strSmybolValue = oResHelper.AddResource(strDLGID, strTemporaryResourceFile, "DIALOG");
							if (strSymbolValue == null) return;				
						}
					}
					if (bFirst)
					{
						// Add header
						RenderAddTemplate(wizard, "control.h", strHeaderFile, selObj, true);	

						// Add HTML
						var strHTMLFile = GetUniqueFileName(strProjectPath, strShortName + ".htm");
						RenderAddTemplate(wizard, "default.htm", strHTMLFile, selObj, false);

						// Add CPP
						RenderAddTemplate(wizard, "control.cpp", strImplFile, selObj, false);
					}

					// close resource file
					oResHelper.CloseResourceFile();
					completedResourceFiles.push(strCurrentResource);
				}
				bFirst = false;
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

function CreateGUIDs()
{
	try
	{	
		// create CLSID
		var strRawGUID = wizard.CreateGuid();
		var strFormattedGUID = wizard.FormatGuid(strRawGUID, 0);
		wizard.AddSymbol("CLSID_REGISTRY_FORMAT", strFormattedGUID);

		// create interface GUID
		strRawGUID = wizard.CreateGuid();
		strFormattedGUID = wizard.FormatGuid(strRawGUID, 0);
		wizard.AddSymbol("INTERFACE_IID", strFormattedGUID);

		// create connection point GUID
		var bConnectionPoint = wizard.FindSymbol("CONNECTION_POINTS");
		if (bConnectionPoint)
		{
			strRawGUID = wizard.CreateGuid();
			strFormattedGUID = wizard.FormatGuid(strRawGUID, 0);
			wizard.AddSymbol("CONNECTION_POINT_IID", strFormattedGUID);
		}

		// create GUID for HTML dispatch interface
		var bHTMLCtl = wizard.FindSymbol("HTML_CONTROL");
		if (bHTMLCtl)
		{
			strRawGUID = wizard.CreateGuid();
			strFormattedGUID = wizard.FormatGuid(strRawGUID, 0);
			wizard.AddSymbol("INTERFACEUI_IID", strFormattedGUID);
	
			// create GUID for HTML coclass
			var bAttributed = wizard.FindSymbol("ATTRIBUTED");
			if (bAttributed)
			{
				strRawGUID = wizard.CreateGuid();
				strFormattedGUID = wizard.FormatGuid(strRawGUID, 0);
				wizard.AddSymbol("OBJECT_UI_GUID", strFormattedGUID);				
			}
		}
	}
	catch(e)
	{
		throw e;
	}
}
