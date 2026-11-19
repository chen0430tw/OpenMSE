// Copyright (c) Microsoft Corporation. All rights reserved.
// Script for ATL Simple Object

function OnFinish(selProj, selObj)
{
	var oCM;
	try
	{
		oCM	= selProj.CodeModel;
		var bDevice = IsDeviceProject(selProj);
		wizard.AddSymbol("DEVICE", bDevice);
		AddDeviceSymbols(false);

		var strShortName = wizard.FindSymbol("SHORT_NAME");
		var L_TRANSACTION_Text = "添加 ATL 简单对象 ";
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
		
		// used only for device projects where platforms may not support DCOM.
		wizard.AddSymbol("SUPPORT_DCOM", false);
		wizard.AddSymbol("SUPPORT_NON_DCOM", false);

		var strProjectName		= wizard.FindSymbol("PROJECT_NAME");
		wizard.AddSymbol("SAFE_IDL_NAME", CreateASCIIName(wizard.FindSymbol("PROJECT_NAME")));
		var strProjectPath		= wizard.FindSymbol("PROJECT_PATH");
		var strTemplatePath		= wizard.FindSymbol("TEMPLATES_PATH");
		var strUpperShortName		= CreateASCIIName(strShortName.toUpperCase());
		var strInterfaceName		= wizard.FindSymbol("INTERFACE_NAME");
		wizard.AddSymbol("UPPER_SHORT_NAME", strUpperShortName);
		var strVIProgID			= wizard.FindSymbol("VERSION_INDEPENDENT_PROGID");
		wizard.AddSymbol("PROGID", strVIProgID.substr(0,37) + ".1");
		var bConnectionPoint		= wizard.FindSymbol("CONNECTION_POINTS");
 		var strClassName		= wizard.FindSymbol("CLASS_NAME");
		var strHeaderFile		= wizard.FindSymbol("HEADER_FILE");
		var strImplFile			= wizard.FindSymbol("IMPL_FILE");
		var strCoClass			= wizard.FindSymbol("COCLASS");
		var bAttributed			= wizard.FindSymbol("ATTRIBUTED");

		var strProjectRC		= GetProjectFile(selProj, "RC", true, false);

		// Create necessary GUIDS
		CreateGUIDs();

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
			var strRGSDCOMFile = GetUniqueFileName(strProjectPath, CreateASCIIName(strShortName + "DCOM") + ".rgs");
			var strRGSID = "IDR_" + strUpperShortName;
			var strRGSDCOMID = "IDR_" + strUpperShortName + "DCOM";

			var bDeviceDCOM = ProjectContainsDCOMPlatform(selProj)
			var bDeviceNonDCOM = ProjectContainsNonDCOMPlatform(selProj)
			
			if (!bDevice)
			{
				RenderAddTemplate(wizard, "object.rgs", strRGSFile, false, false);
			}
			else
			{
				if (bDeviceDCOM)
				{
					wizard.AddSymbol("SUPPORT_DCOM", true);
					RenderAddTemplate(wizard, "object.rgs", strRGSDCOMFile, false, false);
					wizard.AddSymbol("SUPPORT_DCOM", false);
				}
				if (bDeviceNonDCOM)
				{
					wizard.AddSymbol("SUPPORT_NON_DCOM", true);
					RenderAddTemplate(wizard, "object.rgs", strRGSFile, false, false);
					wizard.AddSymbol("SUPPORT_NON_DCOM", false);
				} 
			}    

			wizard.AddSymbol("SUPPORT_DCOM", bDeviceDCOM);
			wizard.AddSymbol("SUPPORT_NON_DCOM", bDeviceNonDCOM);

			if (!bDevice)
			{
				var oResHelper = wizard.ResourceHelper;
				oResHelper.OpenResourceFile(strProjectRC);

				var strSymbolValue = oResHelper.AddResource(strRGSID, strProjectPath + strRGSFile, "REGISTRY");
				if (strSymbolValue == null) return;				
				wizard.AddSymbol("RGS_ID", strSymbolValue.split("=").shift());

				oResHelper.CloseResourceFile();
			}
			else
			{
				var completedResourceFiles = new Array();
				var configs = selProj.Object.Configurations;
				AddDeviceSymbols(false);

				var oResHelper = wizard.ResourceHelper;

				for (var nCntr = 1; nCntr <= configs.Count; nCntr++)
				{
					var config = configs.Item(nCntr);
					var strCurrentResource = GetDeviceResourceFileForConfig(config);
					
					if (completedResourceFiles.join(";").indexOf(strCurrentResource) == -1)
					{
						oResHelper.OpenResourceFile(strCurrentResource);
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
						oResHelper.CloseResourceFile();
						completedResourceFiles.push(strCurrentResource);
					}
					
				}
			}

			// Add connection point support
			if (bConnectionPoint)
				RenderAddTemplate(wizard, "connpt.h", "_" + strInterfaceName + "Events_CP.h", selObj, false);

			// Render objco.idl and insert into strProject.idl
			AddCoclassFromFile(oCM, "objco.idl");

			// Render objint.idl and insert into strProject.idl
			AddInterfaceFromFile(oCM, "objint.idl");
			
			SetMergeProxySymbol(selProj);
		}

		// Add header
		RenderAddTemplate(wizard, "object.h", strHeaderFile, selObj, true);

		// Add CPP
		RenderAddTemplate(wizard, "object.cpp", strImplFile, selObj, false);

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
	}
	catch(e)
	{
		throw e;
	}
}
