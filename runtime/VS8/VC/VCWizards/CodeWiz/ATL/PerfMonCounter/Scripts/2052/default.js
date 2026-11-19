// Copyright (c) Microsoft Corporation. All rights reserved.
function OnFinish(selProj, Class)
{
	var oCM;
	try
	{
		oCM	= selProj.CodeModel;

		var strVariableType = wizard.FindSymbol("VARIABLE_TYPE");
		var strVariableName = wizard.FindSymbol("VARIABLE_NAME");
		var strCounterType = wizard.FindSymbol("COUNTER_TYPE");

		var strNameString = wizard.FindSymbol("NAME_STRING");
		var strHelpString = wizard.FindSymbol("HELP_STRING");

		var strDetailLevel = wizard.FindSymbol("DETAIL_LEVEL");
		var strDefaultScale = wizard.FindSymbol("DEFAULT_SCALE");
		var bDefaultCounter = wizard.FindSymbol("DEFAULT_COUNTER")==true;

		var strParentName = wizard.FindSymbol("PARENT_NAME");
		var strFile = Class.Location(vsCMWhereDefault);

		// open a transaction
		var L_TRANSACTION_Text = "Perf 计数器向导";
		oCM.StartTransaction(L_TRANSACTION_Text);

		if(Class.Name != strParentName)
		{
			var L_Error1_Text = "OnFinish 中的错误上下文对象";
			var oError = new Error(L_Error1_Text);
			SetErrorInfo(oError);
			return;
		}

		var oNewVariable = Class.AddVariable(strVariableName, strVariableType, vsCMAddPositionEnd, vsCMAccessPublic);
		if (Class.Attributes.Find("perf_object"))
		{
			if (bDefaultCounter)
				RemoveDefaultCounter(Class);

			var oAttribute = oNewVariable.AddAttribute("perf_counter", "", vsCMAddPositionEnd);
			oAttribute.AddParameter("namestring", "_T(\"" + QuoteString(strNameString) + "\")", vsCMAddPositionEnd);
			oAttribute.AddParameter("helpstring", "_T(\"" + QuoteString(strHelpString) + "\")", vsCMAddPositionEnd);
			oAttribute.AddParameter("countertype", strCounterType, vsCMAddPositionEnd);
			oAttribute.AddParameter("defscale", strDefaultScale, vsCMAddPositionEnd);
			oAttribute.AddParameter("detail", strDetailLevel, vsCMAddPositionEnd);

			if (bDefaultCounter)
				oAttribute.AddParameter("default_counter", "true", vsCMAddPositionEnd);
		}
		else
		{
			var oMap = Class.Maps.Find("COUNTER");
			if (!oMap)
			{
				oCM.AbortTransaction();

				var L_NoPerfMap_Text = "未能在 CPerfObject 对象中找到 COUNTER 映射";
				var oError = new Error(L_NoPerfMap_Text);
				SetErrorInfo(oError);
				return;
			}

			var oEntry = oMap.AddEntry("DEFINE_COUNTER", vsCMAddPositionEnd);
			oEntry.AddParameter(strVariableName, vsCMAddPositionEnd);
			oEntry.AddParameter("_T(\"" + QuoteString(strNameString) + "\")", vsCMAddPositionEnd);
			oEntry.AddParameter("_T(\"" + QuoteString(strHelpString) + "\")", vsCMAddPositionEnd);
			oEntry.AddParameter(strCounterType, vsCMAddPositionEnd);
			oEntry.AddParameter(strDefaultScale, vsCMAddPositionEnd);

			if (bDefaultCounter)
			{
				// replace the last parameter of the DECLARE_PERF_OBJECT_* entry
				var oDeclare;
				
				oDeclare = Class.Functions.Find("DECLARE_PERF_OBJECT");
				if (!oDeclare)
				{
					oDeclare = Class.Functions.Find("DECLARE_PERF_OBJECT_NO_INSTANCES");
				}
				if (!oDeclare)
				{
					oDeclare = Class.Functions.Find("DECLARE_PERF_OBJECT_EX");
				}
				
				if (oDeclare)
				{
					var strDeclaration = oDeclare.DeclarationText;
					for (var i=strDeclaration.length-1; i>=0; i--)
					{
						if (strDeclaration.substr(i, 1) == ",")
						{
							strDeclaration = strDeclaration.substr(0, i);
							break;
						}
					}
					strDeclaration += ", " + oMap.Entries.Count + ");";
					oDeclare.DeclarationText = strDeclaration;
				}
			}
		}

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

function RemoveDefaultCounter(oClass)
{
	try
	{
		var oMembers = oClass.Members;
		for (i=1; i <= oMembers.Count; i++)
		{
			var oMember = oMembers(i);
			if (oMember.Kind == vsCMElementVariable)
			{
				var oAttribs = oMember.Attributes;
				if (oAttribs)
				{
					var oPerfCounter = oAttribs.Find("perf_counter");
					if (oPerfCounter)
					{
						var oParams = oPerfCounter.Parameters;
						var oDefCounter = oParams.Find("default_counter");
						if (oDefCounter && oDefCounter.DefaultExpression == 1)
							oDefCounter.DefaultExpression = "false";
					}
				}
			}
		}
	}
	catch(e)
	{
		throw e;
	}
}
