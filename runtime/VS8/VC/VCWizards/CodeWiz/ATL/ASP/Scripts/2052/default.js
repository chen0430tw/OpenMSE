// Copyright (c) Microsoft Corporation. All rights reserved.
// Script for ATL ASP Object

function OnFinish(selProj, selObj)
{
	var oCM;
	try
	{
		oCM	= selProj.CodeModel;

		var strShortName = wizard.FindSymbol("SHORT_NAME");

		var L_TRANSACTION_Text = "ÃÌº” ATL ASP ∂‘œÛ ";
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
		wizard.AddSymbol("UPPER_SHORT_NAME", strUpperShortName);
		var strVIProgID			= wizard.FindSymbol("VERSION_INDEPENDENT_PROGID");
		wizard.AddSymbol("PROGID", strVIProgID.substr(0,37) + ".1");
		var bConnectionPoint	= wizard.FindSymbol("CONNECTION_POINTS");
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
			if (oCM.IDLLibraries.Count)
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
			RenderAddTemplate(wizard, "asp.rgs", strRGSFile, false, false);

			var oResHelper = wizard.ResourceHelper;
			oResHelper.OpenResourceFile(strProjectRC);
			var strSymbolValue = oResHelper.AddResource(strRGSID, strProjectPath + strRGSFile, "REGISTRY");
			if (strSymbolValue == null) return;				
			wizard.AddSymbol("RGS_ID", strSymbolValue.split("=").shift());
			oResHelper.CloseResourceFile();

			// Add connection point support
			if (bConnectionPoint)
				RenderAddTemplate(wizard, "connpt.h", strProjectName + "CP.h", selObj, false);

			// Render aspco.idl and insert into strProject.idl
			AddCoclassFromFile(oCM, "aspco.idl");

			// Render aspint.idl and insert into strProject.idl
			AddInterfaceFromFile(oCM, "aspint.idl");

			SetMergeProxySymbol(selProj);
		}

		// Add header first
		RenderAddTemplate(wizard, "asp.h", strHeaderFile, selObj, true);

		// Add CPP
		RenderAddTemplate(wizard, "asp.cpp", strImplFile, selObj, false);

		oCM.CommitTransaction();
		
		var newClass = oCM.Classes.Find(wizard.FindSymbol("CLASS_NAME"));
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
