// Copyright (c) Microsoft Corporation. All rights reserved.
// Script for ATL Dialog

function OnFinish(selProj, selObj)
{
	var oCM;
	try
	{
		oCM = selProj.CodeModel;
		var bDevice = IsDeviceProject(selProj);
		wizard.AddSymbol("DEVICE", bDevice);

		var strShortName = wizard.FindSymbol("SHORT_NAME");
		var L_TRANSACTION_Text = "Ìí¼Ó ATL ¶Ô»°¿ò ";
		oCM.StartTransaction(L_TRANSACTION_Text + strShortName);
		if(!AddATLSupportToProject(selProj))
		{
			oCM.AbortTransaction();
			return;
		}

		var strUpperShortName	= strShortName.toUpperCase();
		strUpperShortName       = CreateSafeName(strUpperShortName);
		wizard.AddSymbol("UPPER_SHORT_NAME", strUpperShortName);
		var strHeaderFile		= wizard.FindSymbol("HEADER_FILE");
		var strImplFile			= wizard.FindSymbol("IMPL_FILE");
		var strTemplatePath		= wizard.FindSymbol("TEMPLATES_PATH");

		var strDLGID = "IDD_" + strUpperShortName;
		var oResHelper = wizard.ResourceHelper;
		var bDBCSCodePage = IsDBCSCharSet(wizard);
		
		if (!bDevice)
		{
			var strProjectRC = GetProjectFile(selProj, "RC", true, false);
			oResHelper.OpenResourceFile(strProjectRC);
			var strRCTemplFile = strTemplatePath + (!bDBCSCodePage ? "\\dialog.rc" : "\\dlg_dbcs.rc");
			var strSymbolValue = oResHelper.AddResource(strDLGID, strRCTemplFile, "DIALOG");
			if (strSymbolValue == null) return;				
			wizard.AddSymbol("IDD_DIALOGID", strSymbolValue.split("=").shift());
			oResHelper.CloseResourceFile();
		}
		else
		{
			var configs = selProj.Object.Configurations;
			var completedResourceFiles = new Array();
			AddDeviceSymbols(false);

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
					
					if (symbol == "POCKETPC2003_UI_MODEL")
					{
						var strRCTemplFile = strTemplatePath + (!bDBCSCodePage ? "\\dialogppc.rc" : "\\dlg_dbcs_ppc.rc");
						var strSymbolValue = oResHelper.AddResource(strDLGID, strRCTemplFile, "DIALOG");
						if (strSymbolValue == null) return;				
						wizard.AddSymbol("IDD_DIALOGID", strSymbolValue.split("=").shift());

						strRCTemplFile = strTemplatePath + "\\dialog_wide.rc";
						strSymbolValue = oResHelper.AddResource(strDLGID + "_WIDE", strRCTemplFile, "DIALOG");
						if (strSymbolValue == null) return;				
						wizard.AddSymbol("IDD_DIALOGID_WIDE", strSymbolValue.split("=").shift());
					}
					else if (symbol == "SMARTPHONE2003_UI_MODEL")
					{
						var strRCTemplFile = strTemplatePath + (!bDBCSCodePage ? "\\dialogsp.rc" : "\\dlg_dbcs_sp.rc");
						var strSymbolValue = oResHelper.AddResource(strDLGID, strRCTemplFile, "DIALOG");
						if (strSymbolValue == null) return;				
						wizard.AddSymbol("IDD_DIALOGID", strSymbolValue.split("=").shift());
					}
					else
					{
						var strRCTemplFile = strTemplatePath + (!bDBCSCodePage ? "\\dialog.rc" : "\\dlg_dbcs.rc");
						var strSymbolValue = oResHelper.AddResource(strDLGID, strRCTemplFile, "DIALOG");
						if (strSymbolValue == null) return;				
						wizard.AddSymbol("IDD_DIALOGID", strSymbolValue.split("=").shift());
					}	

					oResHelper.CloseResourceFile();
					completedResourceFiles.push(strCurrentResource);				
				}
			}
		}

		// Add header
		RenderAddTemplate(wizard, "dialog.h", strHeaderFile, selObj, true);

		// Add CPP
		RenderAddTemplate(wizard, "dialog.cpp", strImplFile, selObj, false);

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
