// Copyright (c) Microsoft Corporation. All rights reserved.
function OnFinish(selProj, Class)
{
	var oCM;
	try
	{
		oCM	= selProj.CodeModel;

		var strClassName = wizard.FindSymbol("CLASS_NAME");

		var strNameString = wizard.FindSymbol("NAME_STRING");
		var strHelpString = wizard.FindSymbol("HELP_STRING");

		var strDetailLevel = wizard.FindSymbol("DETAIL_LEVEL");
		var bNoInstances = wizard.FindSymbol("NO_INSTANCES")==true;

		var strParentName = wizard.FindSymbol("PARENT_NAME");
		var strFile = Class.Location(vsCMWhereDefault);

		// open a transaction
		var L_TRANSACTION_Text = "Perf 对象向导";
		oCM.StartTransaction(L_TRANSACTION_Text);

		if(Class.Name != strParentName)
		{
			var L_Error1_Text = "OnFinish 中的错误上下文对象";
			var oError = new Error(L_Error1_Text);
			SetErrorInfo(oError);
			return;
		}

		var oNewClass;
		if (Class.Attributes.Find("perfmon"))
		{
			oNewClass = Class.AddClass(strClassName, vsCMAddPositionEnd, "", "", vsCMAccessPublic);
			var oAttribute = oNewClass.AddAttribute("perf_object", "", vsCMAddPositionEnd);
			oAttribute.AddParameter("namestring", "\"" + QuoteString(strNameString) + "\"", vsCMAddPositionEnd);
			oAttribute.AddParameter("helpstring", "\"" + QuoteString(strHelpString) + "\"", vsCMAddPositionEnd);
			oAttribute.AddParameter("detail", strDetailLevel, vsCMAddPositionEnd);
			if (bNoInstances)
			{
				oAttribute.AddParameter("no_instances", "true", vsCMAddPositionEnd);
			}
		}
		else
		{
			// add a new object class
			oNewClass = Class.AddClass(strClassName, vsCMAddPositionEnd, "CPerfObject", "", vsCMAccessPublic);

			var oMap = Class.Maps.Find("PERF");
			if (!oMap)
			{
				oCM.AbortTransaction();
				var L_NoPerfMap_Text = "未能在 CPerfMon 对象中找到 PERF 映射";
				var oError = new Error(L_NoPerfMap_Text);
				SetErrorInfo(oError);
				return;
			}

			var oMapItems = oMap.Entries;
			var oChainEntry = oMap.AddEntry("CHAIN_PERF_CATEGORY", vsCMAddPositionEnd);
			oChainEntry.AddParameter(strClassName, vsCMAddPositionEnd);

			// add the id and the declaration
			{
				var oEdit = oNewClass.StartPointOf(vsCMPartBody).CreateEditPoint();

				var str = "public:\r\n";
				if (bNoInstances)
					str += "DECLARE_PERF_CATEGORY_NO_INSTANCES(";
				else
					str += "DECLARE_PERF_CATEGORY(";
				str += strClassName+", "+oMapItems.Count+", "+
						"_T(\""+QuoteString(strNameString)+"\"), "+
						"_T(\""+QuoteString(strHelpString)+"\"), "+
						"-1);\r\n";
				oEdit.Insert(str);
			}
			oCM.Synchronize();

			// DECLARE_PERF_OBJECT should be in,  it's now safe to add this map
			oNewClass.AddMap("COUNTER", strClassName, vsCMAddPositionEnd);
		}

		// Take the new class, cut it, and paste it before the parent class
		// replace this once positioning new elements relative to others is implemented
		var oNewBegin = oNewClass.StartPoint.CreateEditPoint();
		var oNewEnd = oNewClass.EndPoint;

		var oParentBegin = Class.StartPoint.CreateEditPoint();
		oNewBegin.Cut(oNewEnd);
		oParentBegin.Paste();
		oParentBegin.Insert("\r\n\r\n");

		oCM.Synchronize();

		oNewBegin = oCM.Classes.Find(strClassName).StartPoint.CreateEditPoint();
		var oParentEnd = Class.EndPoint;
		oNewBegin.SmartFormat(oParentEnd);

		oCM.Synchronize();
		oCM.CommitTransaction();
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

function QuoteString(str)
{
	try
	{
		var strResult, re;
			re = new RegExp("([\\\\\"])","g");
		strResult = str.replace(re, "\\$1");
		return strResult;
	}
	catch(e)
	{
		throw e;
	}
}
